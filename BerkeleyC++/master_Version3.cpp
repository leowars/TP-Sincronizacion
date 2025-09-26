#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SLAVE1_PORT 10001
#define SLAVE2_PORT 10002
#define SLAVE_HOST "127.0.0.1"

int get_time() {
    // Simula el reloj local del maestro
    return (int)time(nullptr) % 100000;
}

int ask_slave_time(int port) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serv_addr{};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    inet_pton(AF_INET, SLAVE_HOST, &serv_addr.sin_addr);

    if (connect(sock, (sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "[Master] No se pudo conectar al esclavo en puerto " << port << std::endl;
        close(sock);
        return get_time(); // fallback
    }
    send(sock, "TIME?", 5, 0);
    char buffer[64] = {0};
    recv(sock, buffer, 64, 0);
    int slave_time = atoi(buffer);
    close(sock);
    return slave_time;
}

void send_adjustment(int port, int correction) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serv_addr{};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    inet_pton(AF_INET, SLAVE_HOST, &serv_addr.sin_addr);

    if (connect(sock, (sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "[Master] No se pudo conectar al esclavo en puerto " << port << std::endl;
        close(sock);
        return;
    }
    char msg[64];
    sprintf(msg, "ADJUST %d", correction);
    send(sock, msg, strlen(msg), 0);
    close(sock);
}

int main() {
    std::cout << "[Master] Iniciando maestro...\n";
    std::vector<int> slave_ports = {SLAVE1_PORT, SLAVE2_PORT};

    while (true) {
        std::cout << "\n[Master] Nueva ronda de sincronización...\n";
        int local_time = get_time();
        std::cout << "[Master] Mi hora local: " << local_time << std::endl;

        // Consultar esclavos
        std::vector<int> slave_times;
        for (auto port : slave_ports) {
            int t = ask_slave_time(port);
            slave_times.push_back(t);
            std::cout << "[Master] Hora recibida de esclavo en " << port << ": " << t << std::endl;
        }

        // Calcular promedio
        int sum = local_time;
        for (int t : slave_times) sum += t;
        int avg = sum / (slave_times.size() + 1);
        std::cout << "[Master] Promedio calculado: " << avg << std::endl;

        // Enviar ajuste a esclavos
        for (size_t i = 0; i < slave_ports.size(); ++i) {
            int corr = avg - slave_times[i];
            send_adjustment(slave_ports[i], corr);
            std::cout << "[Master] Ajuste enviado a esclavo en " << slave_ports[i] << ": " << corr << std::endl;
        }

        int my_corr = avg - local_time;
        std::cout << "[Master] Mi ajuste local: " << my_corr << std::endl;
        std::cout << "[Master] Esperando 10 segundos...\n";
        std::this_thread::sleep_for(std::chrono::seconds(10));
    }
    return 0;
}
