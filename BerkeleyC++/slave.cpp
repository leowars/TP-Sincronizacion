#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <cstring>
#include <random>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define MY_PORT 10001

int local_time;
std::mutex m;

int get_time() {
    std::lock_guard<std::mutex> lock(m);
    return local_time;
}

void adjust_time(int delta) {
    std::lock_guard<std::mutex> lock(m);
    local_time += delta;
}

void time_server() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(MY_PORT);
    bind(server_fd, (sockaddr*)&address, sizeof(address));
    listen(server_fd, 3);

    std::cout << "[Slave@" << MY_PORT << "] Escuchando conexiones...\n";

    while (true) {
        int new_socket = accept(server_fd, nullptr, nullptr);
        char buffer[64] = {0};
        recv(new_socket, buffer, 64, 0);
        if (strncmp(buffer, "TIME?", 5) == 0) {
            int t = get_time();
            char msg[32];
            sprintf(msg, "%d", t);
            send(new_socket, msg, strlen(msg), 0);
        } else if (strncmp(buffer, "ADJUST", 6) == 0) {
            int delta = atoi(buffer + 7);
            adjust_time(delta);
            std::cout << "[Slave@" << MY_PORT << "] Ajuste recibido: " << delta << ". Nueva hora: " << get_time() << std::endl;
        }
        close(new_socket);
    }
}

void reloj_tictac() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::lock_guard<std::mutex> lock(m);
        local_time += 1;
    }
}

int main() {
    // Inicializa reloj con random offset
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(-50, 50);
    local_time = (int)time(nullptr) % 100000 + dis(gen);

    std::thread server_thread(time_server);
    std::thread tick_thread(reloj_tictac);

    while (true) {
        std::cout << "[Slave@" << MY_PORT << "] Hora local: " << get_time() << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
    server_thread.join();
    tick_thread.join();
    return 0;
}
