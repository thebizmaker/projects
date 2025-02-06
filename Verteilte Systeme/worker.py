import socket
import os
#import sys
import json
import time
import signal
import docker
import tempfile
import shutil
import logging
import threading
import json
#from thrift import Thrift
from thrift.transport import TSocket, TTransport
from thrift.protocol import TBinaryProtocol
from thrift.server import TServer
import paho.mqtt.client as mqtt
from gen_py_matrix.matrix import MatrixService


# Konfiguration des Loggings, damit die Ausgaben in der Konsole erscheinen
logging.basicConfig(
    level=logging.INFO,
    format="%(asctime)s - %(levelname)s - %(message)s"
)
logger = logging.getLogger(__name__)

class Worker:
    def __init__(self, udp_port: int):
        self.udp_port = udp_port
        self.udp_sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.udp_sock.bind(('0.0.0.0', self.udp_port))
        self.worker_name = 'group_d_2-worker-' + self.get_worker_id()
        self.lock = threading.Lock()
        
        self.http_server_host = 'http_server'
        self.http_server_port = 80
        self.rtt_data = []
        self.json_file = "/app/data/http_rtts_data.json"

        self.mqtt_client = mqtt.Client(client_id=self.worker_name, protocol=mqtt.MQTTv5)
        self.clock = 0
        self.request_queue = []
        self.mqtt_port = 1883
        self.broker = "broker"
        self.acks_received = {}
        self.mutex = threading.Lock()

        self.mqtt_client.on_message = self.on_message
        self.mqtt_client.on_connect = self.on_connect
        self.mqtt_client.connect(self.broker, self.mqtt_port, 60)
        self.mqtt_client.loop_start()

        
    def on_connect(self, client, userdata, flags, rc, properties=None) -> None:
        if rc == 0:
            #logger.info(f"Connected to broker with result code {rc}")
            # self.client.subscribe([("request", 2), ("release", 2), ("ack", 2)])
            self.mqtt_client.subscribe("request/#", qos=2)
            self.mqtt_client.subscribe("release/#", qos=2)
            self.mqtt_client.subscribe("ack/#", qos=2)
        else:
            logger.error(f"Failed to connect to broker with result code {rc}")
        
    # msg enthält das Tpoic, Payload und qos
    def on_message(self, client, userdata, msg) -> None:
        with self.mutex:
            topic = msg.topic
            # payload = msg.payload.decode()
            message = json.loads(msg.payload.decode())
            sender = message['worker']
            
            self.clock = max(self.clock, message['clock']) + 1
            #logger.info(f"Received message '{message}' on topic '{topic}'")

            if topic.startswith("request/"):
                #logger.info(f"Received request: {message}")
                self.request_queue.append(message)
                self.request_queue.sort(key=lambda x: (x['clock'], x['worker']))
                self.send_ack(sender)
            elif topic.startswith("release/"):
                #logger.info(f"Received release: {message}")
                self.request_queue = [req for req in self.request_queue if req['worker'] != sender]
            elif topic.startswith("ack/"):
                #logger.info(f"Received ack: {message}")
                self.acks_received[sender] = True
            
    def send_ack(self, target) -> None:
        ack_message = {
            'worker': self.worker_name,
            'ack_for': target,
            'clock': self.clock
        }
        self.mqtt_client.publish(f"ack/{target}", json.dumps(ack_message))
        
    def request_access(self) -> None:
        with self.mutex:
            self.clock += 1
            request_message = {
                'worker': self.worker_name,
                'clock': self.clock
            }
            self.mqtt_client.publish("request", json.dumps(request_message))

            # wait until all acks are received
            while len(self.acks_received) < len(self.request_queue) - 1:
                time.sleep(0.1)
            
    def release_access(self) -> None:
        with self.mutex:
            self.clock += 1
            release_message = {
                'worker': self.worker_name,
                'clock': self.clock
            }
            self.mqtt_client.publish("release", json.dumps(release_message))
            self.acks_received.clear()
        
    def start_udp_server(self) -> None:
        logger.info(f"Worker bereit und wartet auf Healthcheck-Anfragen auf Port {self.udp_port} ...")
        while True:
            data, addr = self.udp_sock.recvfrom(1024)
            self.handle_udp_request(data, addr)
        
    def handle_udp_request(self, data: bytes, addr: tuple) -> None:
        if data.decode() == 'healthcheck':
            #logger.info(f"Healthcheck-Anfrage von Controller mit Adresse {addr} erhalten auf Port {self.udp_port}")
            self.udp_sock.sendto(bytes('available', "utf-8"), addr)
        elif data.decode() == 'stop':
            #logger.info(f"Stop-Anfrage von Controller mit Adresse {addr} erhalten auf Port {self.udp_port}")
            self.udp_sock.close()
        elif 'Berechnungen fertig' in data.decode():
            #row_a_and_col_b = data.decode().split(':')[1]
            #row_a, col_b = row_a_and_col_b.split('x')
            #logger.info(f"Keine Berechnung mehr vom Controller")
            self.send_post_request('Message', 'Berechnung fertig')
            self.udp_sock.sendto(bytes('OK', "utf-8"), addr)
            
    def write_rtts_in_json_file(self, rtt_data: list) -> None:
        """RTTs in eine JSON-Datei sicher schreiben"""
        data_to_write = {
            'Worker': self.worker_name[-1],
            'RTTs': rtt_data
        }

        # make sure the file exists and is not empty
        if not os.path.exists(self.json_file) or os.path.getsize(self.json_file) == 0:
            with open(self.json_file, "w", encoding='utf-8') as file:
                json.dump([], file)

        # read the existing data from the file
        try:
            with open(self.json_file, "r", encoding='utf-8') as file:
                data = json.load(file)
        except (FileNotFoundError, json.JSONDecodeError):
            data = []

        # verify if the worker already exists in the data and update or add the RTTs
        existing_entry = next((item for item in data if item['Worker'] == self.worker_name[-1]), None)

        if existing_entry:
            existing_entry['RTTs'] = rtt_data
        else:
            data.append(data_to_write)

        # secure write the data back to the file
        with tempfile.NamedTemporaryFile("w", delete=False, encoding='utf-8') as temp_file:
            json.dump(data, temp_file, indent=4)
            temp_filename = temp_file.name

        shutil.move(temp_filename, self.json_file)

        #logger.info(f"RTT gespeichert in {self.json_file}.")
        
    def send_http_request(self, request, retries=5, timeout=5) -> str:
        last_exception = None
        for attempt in range(1, retries + 1):
            try:
                logger.info(f"Versuch {attempt}/{retries}: Verbindung zum Server herstellen...")
                with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as client_socket:
                    client_socket.settimeout(timeout)  # Setzt ein Timeout
                    start_time = time.time()
                    client_socket.connect((self.http_server_host, self.http_server_port))  # Verbindung aufbauen
                    client_socket.send(request.encode())  # Anfrage senden
                    response = client_socket.recv(1024).decode()  # Antwort empfangen
                    end_time = time.time()
                    #logger.info(f"Antwort erhalten: {response}")
                    rtt = round((end_time - start_time) * 1000, 2)
                    #logger.info(f"RTT: {rtt} ms")
                    with self.lock:
                        self.rtt_data.append(rtt)
                        self.write_rtts_in_json_file(self.rtt_data)
                    return response  # Erfolgreiche Antwort zurückgeben
            except socket.timeout:
                logger.warning(f"Timeout beim Versuch {attempt}/{retries}.")
                last_exception = Exception("Timeout beim Warten auf den Server.")
            except socket.error as e:
                logger.error(f"Fehler beim Verbinden zum Server: {e}")
                last_exception = e
        # Wenn alle Versuche fehlschlagen, Exception auslösen
        raise last_exception
    
    def send_post_request(self, key, value) -> str:
        body =json.dumps({key: value})
        request = (f"POST / HTTP/1.1\r\nHost: {self.http_server_host}\r\nContent-Type: application/json"
                    f"\r\nContent-Length: {len(body.encode('utf-8'))}\r\n\r\n{body}")
        return self.send_http_request(request)

    def send_get_request(self)  -> str:
        request = f"GET / HTTP/1.1\r\nAccept: application/json\r\nHost: {self.http_server_host}\r\n\r\n"
        return self.send_http_request(request)
    
    def startRPCServer(self) -> None:
        handler = MatrixHandler(self)
        processor = MatrixService.Processor(handler)
        transport = TSocket.TServerSocket(port=9090)
        tfactory = TTransport.TBufferedTransportFactory()
        pfactory = TBinaryProtocol.TBinaryProtocolFactory()
        server = TServer.TSimpleServer(processor, transport, tfactory, pfactory)
        logger.info("Starting the RPC server...")
        server.serve()
        logger.info("RPC server stopped.")
        
    def get_worker_id(self) -> str:
        client = docker.from_env()
        container = client.containers.get(socket.gethostname())
        return container.attrs['Config']['Labels']['com.docker.compose.container-number']
        
# Klasse zur Verarbeitung der Matrix
class MatrixHandler:
    def __init__(self, worker: Worker):
        self.worker = worker
    def computeMatrix(self, matrix: MatrixService.Matrix) -> int:
        #logger.info(f'Computing matrix {matrix.data}...')
        row, col, final = matrix.row, matrix.col, matrix.final_dims
        result = sum(a * b for a, b in matrix.data)
        result = {
            'row': row,
            'col': col,
            'result': result,
            'final_dims': final
        }
        return json.dumps(result)
    
    def storeResult(self, result: str, worker: str) -> None:
        self.worker.request_access()
        logger.info(f'Storing result:{result} in DB...')
        if(self.worker.send_post_request(worker, json.loads(result))):
            logger.info(f'Result:{result} from Worker {worker} succesfully stored in DB !')
        self.worker.release_access()

# Json-Datei leeren und Worker beenden
def signal_handler(sig, frame):
    with open("/app/data/http_rtts_data.json", "w") as file:
        json.dump([], file)
    exit(0)
            
if __name__ == "__main__":
    signal.signal(signal.SIGINT, signal_handler)
    signal.signal(signal.SIGTERM, signal_handler)
    
    port = int(os.getenv("WORKER_PORT", '12345'))
    worker = Worker(port)
    
    thread_udp = threading.Thread(target=worker.start_udp_server)
    thread_rpc = threading.Thread(target=worker.startRPCServer)
    
    thread_udp.start()
    thread_rpc.start()
    
    thread_udp.join()
    thread_rpc.join()