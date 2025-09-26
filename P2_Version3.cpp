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
void send_message(const std::string& dest_ip, int dest_port, const std::string& msg) {
    WSADATA wsa;
    SOCKET sock;
    struct sockaddr_in server;

    WSAStartup(MAKEWORD(2,2),&wsa);
    sock = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_addr.s_addr = inet_addr(dest_ip.c_str());
    server.sin_family = AF_INET;
    server.sin_port = htons(dest_port);

    connect(sock, (struct sockaddr *)&server, sizeof(server));
    send(sock, msg.c_str(), msg.size(), 0);
    closesocket(sock);
    WSACleanup();
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
    int id = 2;
    int my_port = 5002;
    std::string next_ip = "127.0.0.1";
    int next_port = 5003;

    std::cout << "Proceso P2 corriendo en puerto " << my_port << std::endl;

    std::string recv_msg;
    std::cout << "P2 esperando mensaje..." << std::endl;
    listen_message(my_port, recv_msg);

    int m_clock, m_id;
    std::string m_type;
    parse_message(recv_msg, m_clock, m_id, m_type);
    update_clock(m_clock);

    std::cout << "P2 recibe mensaje de P" << m_id << " con clock " << m_clock << std::endl;
    std::cout << "P2 ejecutando su accion. Clock local: " << lamport_clock << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(1));

    increment_clock();
    std::string msg = serialize(lamport_clock, id, "request");
    std::cout << "P2 envia mensaje a P3" << std::endl;
    send_message(next_ip, next_port, msg);

    std::cout << "P2 finaliza." << std::endl;
    return 0;
}