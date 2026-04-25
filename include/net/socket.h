#pragma once

class Socket {
public:
    Socket();

    explicit Socket(int fd);

    Socket(const Socket&) = delete;
    Socket& operator=(const Socket&) = delete;

    Socket(Socket&& other) noexcept;
    Socket& operator=(Socket&& other) noexcept;

    ~Socket();

    int fd() const;

    bool valid() const;

    int release();

    void reset(int new_fd = -1);
    
private:
    int fd_;
};