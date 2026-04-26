#include <sys/types.h>
#include <sys/socket.h>
#include <string>

ssize_t send_all(int fd, const char* message, size_t size) {
    size_t total = 0;

    while (total < size) {
        ssize_t sent = send(fd, message + total, size - total, 0);

        if (sent <= 0) {
            return -1;
        }

        total += sent;
    }

    return total;
}

std::string receive_all(int fd) {
    std::string res;
    char buffer[1024];

    while (true) {
        ssize_t received = recv(fd, buffer, sizeof(buffer), 0);

        if (received > 0) {
            res.append(buffer, received);
        }
        else if (received = 0) {
            break;
        }
        else {
            return "";
        }
    }

    return res;
}