#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include "../../include/net/socket.h"

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: << argv[0] << " "<host> <port> <message>" << std::endl;
        return 1; 
    }

    const char* host = argv[1];
    const char* port = argv[2];
    const char* message = argv[3];

    addrinfo hints{};
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    addrinfo* result = nullptr;

    int res = getaddrinfo(host, port, &hints, &result);
    if (res != 0) {
        std::cerr << "getaddrinfo: " << gai_strerror(res) << std::endl;
        return 1;
    }

    Socket sock;

    for (addrinfo* a = result; a; a = a->ai_next) {
        int fd = socket(a->ai_family, a->ai_socktype, a->ai_protocol);
        if (fd == -1) {
            continue;
        }

        sock.reset(fd);

        if (connect(sock.fd(), a->ai_addr, a->ai_addrlen) == 0) {
            break;
        }

        sock.reset();
    }

    freeaddrinfo(result);

    if (!sock.valid()) {
        std::cerr << "Couldn't connnect" << std::endl;
        return 1;
    }

    std::cout << "Connected to " << host << ":" << port << std::endl;

    ssize_t sent = send(sock.fd(), message, std::strlen(message), 0);
    if (sent == -1) {
        std::cerr << "send failed" << std::endl;
        return 1;
    }

    char buffer[1024];
    ssize_t received = recv(sock.fd(), buffer, sizeof(buffer) - 1, 0);

    if (received > 0) {
        buffer[received] = '\0';
        std::cout << "Received: " << buffer << std::endl; 
    }
    else if (received == 0) {
        std::cout << "Connection closed by server" << std::endl;
    }
    else {
        std::cerr << "recv failed" << std::endl;
    }

    return 0;
}