import socket
import time
import logging
import random
import queue
import threading
import os
import json
from thrift import Thrift
from concurrent.futures import ThreadPoolExecutor
from thrift.transport import TSocket, TTransport
from thrift.protocol import TBinaryProtocol
from thrift.server import TServer
from gen_py_matrix.matrix import MatrixService


logging.basicConfig(
    level=logging.INFO,
    format="%(asctime)s - %(levelname)s - %(message)s"
)
logger = logging.getLogger(__name__)

class Controller:
    def __init__(self, matrix_a: list, matrix_b: list):
        #self.final_dimensions = (len(matrix_a), len(matrix_b[0]))
        self.task_generator = self.separate_matrix(matrix_a, matrix_b)
        worker_count = int(os.getenv("WORKER_COUNT", "3"))
        self.worker_queue = self.generate_workers_list(worker_count)
        
        self.lock = threading.Lock()
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.sock.settimeout(5)
        
        self.rtts: list[dict] = []
        self.old_task = dict()
        #self.udp_threads: list[threading.Thread] = []
        
    def start_healthchecks_and_send_task(self, message: str, worker_address: tuple[str, int], task: dict=dict()) -> None:
        address, port = worker_address
        try:
            #logger.info(f"Sending '{message}' to {address}:{port} ...")
            start_time = time.time()
            self.sock.sendto(bytes(message, "utf-8"), (address, port))
            data, _ = self.sock.recvfrom(1024)
            end_time = time.time()
            
            # Wenn der Worker verfügbar ist, wird die Matrix gesendet
            if data.decode() == 'available':
                # Matrix an Worker senden
                self.send_task(address, task)
                rtt = (end_time - start_time) * 1000  # RTT in Millisekunden
                rtt = round(rtt, 2)
                # RTT in die Liste einfügen
                with self.lock:
                    count = 0
                    for item in self.rtts:
                        if item['Worker'] == address[-1]:
                            item['RTT'].append(rtt)
                            break
                        count += 1
                    # Wenn die Adresse noch nicht in der Liste ist, wird sie hinzugefügt
                    if count == len(self.rtts):
                        data_to_append = {
                            'Worker': address[-1],
                            'RTT': []
                        }
                        data_to_append['RTT'].append(rtt)
                        self.rtts.append(data_to_append)
                #logger.info(f"Antwort von Worker {address}:{port} erhalten: {data.decode()}")
                #logger.info(f"Round-Trip-Time: {rtt} ms\n")
                # Ping an den Worker senden und RTT zurückgeben
                # os.system(f"ping -c 1 {address}")
            # else:
            #     logger.info(f"Antwort vom Worker {address}:{port}: {data.decode()}")
        except socket.timeout:
            with self.lock:
                self.old_task = task
            logger.info(f"Worker:{address} not available !")
        finally:
            return
            
    def get_rtts(self) -> list[dict]:
        return self.rtts

    def close_socket(self) -> None:
        self.sock.close()
        
    # Funktion zum Trennen der Matrix in verschiedene zu berechnenden Teilen
    def separate_matrix(self, matrix_a: list, matrix_b: list): #-> queue.Queue[dict]:
        row_a = len(matrix_a)
        col_a = len(matrix_a[0])
        col_b = len(matrix_b[0])

        if col_a != len(matrix_b):
            logger.error("Matrix A and B cannot be multiplied!")
            return None
        # result_queue: queue.Queue[dict] = queue.Queue()
        # erstelle eine Liste von Aufgaben, die von den Workern erledigt werden sollen
        for i in range(row_a):
            for j in range(col_b):
                task = {
                    'row': i,
                    'col': j,
                    'matrix': [(matrix_a[i][k], matrix_b[k][j]) for k in range(col_a)],
                    'final_dims': f'{row_a}x{col_b}'
                }
                yield task # result_queue.put(task)
        # return result_queue
    
    # Funktion zum Senden der Matrix an den Worker
    def send_task(self, worker_address: str, task: dict) -> None:
        address = worker_address
        task_row = task['row']
        task_col = task['col']
        matrix = task['matrix']
        final = task['final_dims']
        
        transport = TSocket.TSocket(address, 9090)
        transport = TTransport.TBufferedTransport(transport)
        protocol = TBinaryProtocol.TBinaryProtocol(transport)
        client = MatrixService.Client(protocol)
        try:
            transport.open()
            #logger.info(f"Sending matrix to {address} ...")
            matrix = MatrixService.Matrix(row=task_row, col=task_col, worker=address, data=matrix, final_dims=final)
            result = client.computeMatrix(matrix)
            client.storeResult(result, address)
            #transport.close()
            with self.lock:
                self.old_task = dict()
            #logger.info(f"Matrix: {json.dumps(task)} successfully sent to {address} !")
        except Thrift.TException:
            with self.lock:
                self.old_task = task
                logger.info(f"Worker:{address} not available !")
        finally:
            transport.close()
            return
    
    # Funktion zum Generieren der Worker-Liste
    def generate_workers_list(self, num_workers: int) -> queue.Queue[tuple[str, int]]:
        result: queue.Queue[tuple[str, int]] = queue.Queue()
        if num_workers < 1:
            return result
        base_port = int(os.getenv("WORKER_PORT", "12345"))
        base_name = "group_d_2-worker"  # Name des Workers
    
        # Erstellen der Worker-Adressen
        for i in range(1, num_workers + 1):
            host = f"{base_name}-{i}"  # Alle Worker nutzen denselben Basisnamen im Netzwerk
            port = base_port  # Jeder Worker hat eine eindeutige Portnummer
            result.put((host, port))
        return result

    def share_tasks(self) -> list[dict]:
        start = time.time()
        with ThreadPoolExecutor(max_workers=10) as executor:
            while not self.worker_queue.empty():
                #if self.task_queue.empty():
                    #break
                try:
                    new_task = next(self.task_generator) if len(self.old_task) == 0 else self.old_task
                except StopIteration:
                    break
                worker_address = self.worker_queue.get()
                #new_task = self.task_queue.get() if len(self.old_task) == 0 else self.old_task
                # thread = threading.Thread(
                #     target=self.start_healthchecks_and_send_task,
                #     args=('healthcheck', worker_address, new_task,)
                # )
                # thread.start()
                executor.submit(self.start_healthchecks_and_send_task, 'healthcheck', worker_address, new_task)
                #self.udp_threads.append(thread)
                self.worker_queue.put(worker_address)

        # for thread in self.udp_threads:
        #     thread.join()
        
        end = time.time()
        logger.info(f"Time taken to send all tasks: {(end - start) * 1000} ms")
            
        for worker_address in list(self.worker_queue.queue):
            #row_a, col_b = self.final_dimensions
            self.start_healthchecks_and_send_task(f'Berechnungen fertig', worker_address)

        return self.get_rtts()
    
    def write_in_json_file(self, rtts: list[dict], filename: str) -> None:
        with open(filename, 'w') as file:
            json.dump(rtts, file, indent=4)
        logger.info(f"RTTs in {filename} gespeichert !")
        





# Funktion zum Generieren zwei zufälliger Matrizen
def generate_2_random_matrix_to_multiply(min_size=2, max_size=5, value_range=(0, 10)):
    """
    Args:
        min_size (int): minimum size for rows or columns of the matrix
        max_size (int): maximum size for rows or columns of the matrix
        value_range (tuple): range of values for the matrix elements

    Returns:
        tuple: (matrix_1, matrix_2, final_dimensions)
    """
    # generate random dimensions for the matrices
    rows_1 = random.randint(min_size, max_size)  # number of rows of the first matrix
    cols_1 = random.randint(min_size, max_size)  # nomber of columns of the first matrix(= number of rows of the second matrix)
    rows_2 = cols_1  # number of rows of the second matrix
    cols_2 = random.randint(min_size, max_size)  # number of columns of the second matrix
    
    # generate random values for the matrices
    matrix_1 = [[random.randint(*value_range) for _ in range(cols_1)] for _ in range(rows_1)]
    matrix_2 = [[random.randint(*value_range) for _ in range(cols_2)] for _ in range(rows_2)]
    
    return matrix_1, matrix_2


def init_matrix(rows: int, columns: int) -> list[list[int]]:
    return [[random.randint(0, 10) for _ in range(columns)] for _ in range(rows)]
        

if __name__ == "__main__":
    
    matrix_a , matrix_b = generate_2_random_matrix_to_multiply()
    #matrix_a = init_matrix(1500, 1000)
    #matrix_b = init_matrix(1000, 1500)
    
    controller = Controller(matrix_a, matrix_b)
    rtts = controller.share_tasks()
    controller.write_in_json_file(rtts, '/app/data/udp_rtts.json')
    controller.close_socket()
    while True:
        running = True
    
    #print(generate_2_random_matrix_to_multiply())
        

