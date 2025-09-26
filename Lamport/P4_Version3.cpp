#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <mutex>
#pragma comment(lib, "ws2_32.lib")

std::mutex clock_mutex;
int lamport_clock = 0;
std::string serialize(int clock, int id, const std::string& type) {
    return std::to_string(clock) + "," + std::to_string(id) + "," + type;
}
void parse_message(const std::string& msg, int& clock, int& id, std::string& type) {
    size_t p1 = msg.find(",");
    size_t p2 = msg.find(",", p1+1);
    clock = std::stoi(msg.substr(0, p1));
    id = std::stoi(msg.substr(p1+1, p2-p1-1));
    type = msg.substr(p2+1);
}
void send_message(const std::string&, int, const std::string&) {
    // P4 no envía a nadie
}
void listen_message(int my_port, std::string& out_msg) {
    WSADATA wsa;
    SOCKET s, new_socket;
    struct sockaddr_in server, client;
    int c;
    char buffer[1024] = {0};

    WSAStartup(MAKEWORD(2,2),&wsa);
    s = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(my_port);

    bind(s, (struct sockaddr *)&server, sizeof(server));
    listen(s, 3);

    c = sizeof(struct sockaddr_in);
    new_socket = accept(s, (struct sockaddr *)&client, &c);

    int valread = recv(new_socket, buffer, 1024, 0);
    out_msg = std::string(buffer, valread);

    closesocket(new_socket);
    closesocket(s);
    WSACleanup();
}
void increment_clock() {
    std::lock_guard<std::mutex> lock(clock_mutex);
    lamport_clock++;
}
void update_clock(int msg_clock) {
    std::lock_guard<std::mutex> lock(clock_mutex);
    lamport_clock = std::max(lamport_clock, msg_clock) + 1;
}
int main() {
    int id = 4;
    int my_port = 5004;

    std::cout << "Proceso P4 corriendo en puerto " << my_port << std::endl;

    std::string recv_msg;
    std::cout << "P4 esperando mensaje..." << std::endl;
    listen_message(my_port, recv_msg);

    int m_clock, m_id;
    std::string m_type;
    parse_message(recv_msg, m_clock, m_id, m_type);
    update_clock(m_clock);

    std::cout << "P4 recibe mensaje de P" << m_id << " con clock " << m_clock << std::endl;
    std::cout << "P4 ejecutando su accion. Clock local: " << lamport_clock << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "P4 finaliza el ciclo." << std::endl;
    return 0;
}