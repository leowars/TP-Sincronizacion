import socket
import time

HOST = 'localhost'
PORT = 5000

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.bind((HOST, PORT))
    s.listen()
    print(f"Servidor de tiempo escuchando en {HOST}:{PORT}")
    while True:
        conn, addr = s.accept()
        with conn:
            data = conn.recv(1024)
            if data:
                # Enviar tiempo actual del servidor (en segundos)
                current_time = time.time()
                conn.sendall(str(current_time).encode())