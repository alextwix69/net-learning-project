#include <iostream>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "../../include/net/socket.h"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << "<bind_address> <port>" << std::endl;
        return 1;
    }

    const char* bind_address = argv[1];
    int port = std::stoi(argv[2]);

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        std::cerr << "socket failed" << std::endl;
        return 1;
    }

    Socket server(fd);

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    if (inet_pton(AF_INET, bind_address, &addr.sin_addr) <= 0) {
        std::cerr << "invalid address" << std::endl;
        return 1;
    }

    if (bind(server.fd(), (sockaddr*)&addr, sizeof(addr)) < 0) {
        std::cerr << "bind failed" << std::endl;
        return 1;
    }

    if (listen(server.fd(), 10) < 0) {
        std::cerr << "listen failed" << std::endl;
        return 1;
    }

    sockaddr_in client_addr{};
    socklen_t client_len = sizeof(client_addr);

    int client_fd = accept(server.fd(), (sockaddr*)&client_addr, &client_len);
    if (client_fd < 0) {
        std::cerr << "accept failed" << std::endl;
        return 1;
    }

    Socket client(client_fd);

    std::cout << "client connected" << std::endl;

    char buffer[1024];

    ssize_t received = recv(client.fd(), buffer, sizeof(buffer), 0);

    if (received > 0) {
        send(client.fd(), buffer, received, 0);
    }

    std::cout << "Done" << std::endl;

    return 0;
}