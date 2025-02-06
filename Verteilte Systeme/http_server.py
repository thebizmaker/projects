import socket
import json
import threading
import logging
from concurrent.futures import ThreadPoolExecutor

# Logging-Konfiguration
logging.basicConfig(
    level=logging.INFO,
    format="%(asctime)s - %(levelname)s - %(message)s"
)
logger = logging.getLogger(__name__)

class HTTP_Server:
    def __init__(self):
        self.db = []
        self.temp_db = []
        self.result_matrix = []
        self.lock = threading.Lock()

    def add(self, data, temp: bool = False) -> None:
        if temp:
            self.temp_db.append(data)
            #logger.info(f"Neue temporäre Zeile hinzugefügt: {data}")
            return
        self.db.append(data)
        #logger.info(f"Neue Zeile hinzugefügt: {data}")

    def get_all(self) -> list:
        return list(self.db)
    
    def form_final_matrix(self, row_a: int, col_b: int) -> None:
        self.result_matrix = [[0] * col_b for _ in range(row_a)]
    
    def store_in_final_matrix(self, data: dict) -> None:
        #for dict1 in data:
        row = data[next(iter(data))]['row']
        col = data[next(iter(data))]['col']
        self.result_matrix[row][col] = data[next(iter(data))]['result']
    
    def format_data(self):
        self.result_matrix  = ['['+', '.join(map(str, self.result_matrix[i]))+']' for i in range(len(self.result_matrix))]
        self.add(self.result_matrix)
        #return self.result_matrix

def handle_client(client_socket, db: HTTP_Server) -> None:
    try:
        # Empfang der Anfrage
        request = client_socket.recv(1024).decode('utf-8')
        #logger.info(f"Anfrage erhalten:\n{request}")  # Debugging line to see the raw request

        # HTTP-Header analysieren
        headers = request.split('\r\n')
        logger.debug(f"Headers:\n{headers}")  # Debugging line to check headers

        # Check if the request line has three parts
        try:
            method, path, _ = headers[0].split(' ')
        except ValueError:
            logger.warning(f"Ungültige Anfragezeile: {headers[0]}")
            return  # Handle error or send 400 Bad Request response
        
        # GET-Anfrage bearbeiten
        if method == "GET" and path == "/":
            accept = None
            for header in headers:
                if header.lower().startswith("accept"):
                    accept = header.split(":")[1].strip()
                    break
            if accept and 'application/json' in accept or '*/*' in accept:
                # matrix = db.get_all()[0]
                response_data = {
                    # "rows": len(matrix),
                    # "columns": len(matrix[0]),
                    "result": db.get_all()[0]
                }
                response_body = json.dumps(response_data, indent=4)
                response = (
                    "HTTP/1.1 200 OK\r\n"
                    "Content-Type: application/json\r\n"
                    f"Content-Length: {len(response_body.encode('utf-8'))}\r\n"
                    "connection: close\r\n"
                    "\r\n"
                    f"{response_body}"
                )
                client_socket.sendall(response.encode('utf-8'))
                #logger.info("GET-Anfrage erfolgreich bearbeitet.")
            elif accept and 'text/html' in accept:
                response_body = "<html><body><h1>Ergebnis</h1><p>Matrix: " + str(db.get_all()[0]) + "</p></body></html>"
                response = (
                    "HTTP/1.1 200 OK\r\n"
                    "Content-Type: text/html\r\n"
                    f"Content-Length: {len(response_body.encode('utf-8'))}\r\n"
                    "connection: close\r\n"
                    "\r\n"
                    f"{response_body}"
                )
                client_socket.sendall(response.encode('utf-8'))
                logger.info("GET-Anfrage erfolgreich bearbeitet.")
            else:
                response_body = json.dumps({"error": "Not Acceptable"})
                response = (
                    "HTTP/1.1 406 Not Acceptable\r\n"
                    "Content-Type: application/json\r\n"
                    f"Content-Length: {len(response_body.encode('utf-8'))}\r\n"
                    "connection: close\r\n"
                    "\r\n"
                    f"{response_body}"
                )
                client_socket.sendall(response.encode('utf-8'))
                logger.warning("Nicht unterstützter Accept-Header!")

        # POST-Anfrage bearbeiten
        elif method == "POST" and path == "/":
            # Extrahiere den Body der Anfrage
            content_length = 0
            content_type = ""
            for header in headers:
                if header.lower().startswith("content-length"):
                    # 1. Teile den String 'header' an jedem ':' und erhalte eine Liste
                    key_and_value = header.split(":")
                    # 2. Nimm das zweite Element der Liste (Index 1) und entferne Leerzeichen
                    value = key_and_value[1].strip()
                    # 3. Konvertiere das bereinigte Element in eine Ganzzahl
                    content_length = int(value)
                if header.lower().startswith("content-type"):
                    content_type = header.split(":")[1].strip()

            body = request.split("\r\n\r\n")[1]  # Body nach den Headers
            if len(body.encode('utf-8')) < content_length:
                body += client_socket.recv(content_length - len(body.encode('utf-8'))).decode('utf-8')

            if content_type == "application/json" and len(body.encode('utf-8')) == content_length:
                try:
                    # Füge die Daten in die DB ein
                    keys_to_search = ['row', 'col', 'result']
                    data = json.loads(body)
                    count = 0
                    for key in keys_to_search:
                        if key in data[next(iter(data))]:
                            count += 1
                    if count == 3:
                        if db.result_matrix == []:
                            dims = data[next(iter(data))]['final_dims']
                            row_a, col_b = dims.split('x')
                            db.form_final_matrix(int(row_a), int(col_b))
                        db.store_in_final_matrix(data)
                        #db.add(data, temp=True)
                    else:
                        if 'Message' in data:
                            db.format_data()
                            # row_a, col_b = data['Berechnung fertig'].split('x')
                            # data = db.collect_data(int(row_a), int(col_b))
                            # if data not in db.get_all():
                            #     db.add(data)
                        else:
                            db.add(data)
                        
                    response_body = json.dumps({"message": "Data added successfully"})
                    response = (
                        "HTTP/1.1 200 OK\r\n"
                        "Content-Type: application/json\r\n"
                        f"Content-Length: {len(response_body.encode('utf-8'))}\r\n"
                        "\r\n"
                        f"{response_body}"
                    )
                    logger.info("POST-Anfrage erfolgreich bearbeitet.")
                except json.JSONDecodeError:
                    response_body = json.dumps({"error": "Invalid JSON"})
                    response = (
                        "HTTP/1.1 400 Bad Request\r\n"
                        "Content-Type: application/json\r\n"
                        f"Content-Length: {len(response_body.encode('utf-8'))}\r\n"
                        "\r\n"
                        f"{response_body}"
                    )
                    logger.warning("Ungültiges JSON in der Anfrage.")
                client_socket.sendall(response.encode('utf-8'))
            else:
                # Fehlerhafte Anfrage
                response_body = json.dumps({"error": "Bad Request"})
                response = (
                    "HTTP/1.1 400 Bad Request\r\n"
                    "Content-Type: application/json\r\n"
                    f"Content-Length: {len(response_body.encode('utf-8'))}\r\n"
                    "\r\n"
                    f"{response_body}"
                )
                client_socket.sendall(response.encode('utf-8'))
                logger.warning("Fehlerhafte Anfrage!")
        else:
            # Nicht unterstützte Methode
            response_body = json.dumps({"error": "Not Implemented"})
            response = (
                "HTTP/1.1 501 Not Implemented\r\n"
                "Content-Type: application/json\r\n"
                f"Content-Length: {len(response_body.encode('utf-8'))}\r\n"
                "\r\n"
                f"{response_body}"
            )
            client_socket.sendall(response.encode('utf-8'))
            logger.warning(f"Nicht unterstützte Methode: {method}")
    finally:
        # Verbindung schließen
        client_socket.close()
        logger.info("Verbindung geschlossen!")


def start_server(host, port) -> None:

    db = HTTP_Server()
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.bind((host, port))
    server_socket.listen(10)
    logger.info(f"Server gestartet unter http://{host}:{port}")

    #with ThreadPoolExecutor(max_workers=500) as executor:
    while True:
        client_socket, addr = server_socket.accept()
        logging.info(f"Connection from {addr}")
        #executor.submit(handle_client, client_socket, db)
        client_thread = threading.Thread(target=handle_client, args=(client_socket, db), daemon=True)
        client_thread.start()



if __name__ == "__main__":
    start_server("0.0.0.0", 80)
