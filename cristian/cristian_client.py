import socket
import time

HOST = 'localhost'
PORT = 5000

def sincronizar_cristian():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        t0 = time.time()  # Tiempo antes de enviar la petición
        s.connect((HOST, PORT))
        s.sendall(b"GET_TIME")
        data = s.recv(1024)
        t1 = time.time()  # Tiempo al recibir la respuesta

        server_time = float(data.decode())
        rtt = t1 - t0
        offset = server_time + rtt / 2 - t1  # Desfase estimado

        print(f"RTT: {rtt:.6f} segundos")
        print(f"Hora del servidor: {time.ctime(server_time)}")
        print(f"Hora local antes: {time.ctime(t1)}")
        print(f"Desfase estimado: {offset:.6f} segundos")
        # Ajuste del reloj local: solo mostramos el cálculo, no cambiamos el reloj real
        print(f"Hora local sincronizada estimada: {time.ctime(t1 + offset)}")

if __name__ == "__main__":
    sincronizar_cristian()