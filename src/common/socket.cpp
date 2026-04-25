#include "../../include/net/socket.h"
#include <unistd.h>

Socket::Socket() : fd_(-1) {}

Socket::Socket(int fd) : fd_(fd) {}

Socket::Socket(Socket&& other) noexcept : fd_(other.fd_) {
    other.fd_ = -1;
}

Socket& Socket::operator=(Socket&& other) noexcept {
    if (this != &other) {
        reset();

        fd_ = other.fd_;
        other.fd_ = -1;
    }

    return *this;
}

Socket::~Socket() {
    if (fd_ != -1) {
        close(fd_);
    }
}

int Socket::fd() const{
    return fd_;
}

bool Socket::valid() const {
    return fd_ != -1;
}

int Socket::release() {
    int temp = fd_;
    fd_ = -1;
    return temp;
}

void Socket::reset(int new_fd) {
    if (fd_ != -1) {
        close(fd_);
    }
    fd_ = new_fd;
}