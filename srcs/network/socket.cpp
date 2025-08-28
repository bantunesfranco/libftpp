/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   socket.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: bfranco <bfranco@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/30 11:08:32 by bfranco       #+#    #+#                 */
/*   Updated: 2025/08/29 01:10:28 by bfranco       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "core/network/socket.hpp"
#include <unistd.h>
#include <cstring>
#include <string>

Socket::Socket() : _fd(-1) {}

Socket::~Socket() {
    close();
}

Socket::Socket(Socket&& other) : _fd(other._fd) {
    other._fd = -1;
}

Socket& Socket::operator=(Socket&& other) {
    if (this != &other) {
        close();
        _fd = other._fd;
        other._fd = -1;
    }
    return *this;
}

bool Socket::create() {
    _fd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    return _fd != -1;
}

bool Socket::bind(std::string_view ip, uint16_t port, sockaddr_in* addr) {
    sockaddr_in default_addr{};

    if (addr) default_addr = *addr;
    default_addr.sin_family = AF_INET;
    default_addr.sin_port = htons(port);

    if (::inet_pton(AF_INET, ip.data(), &default_addr.sin_addr) <= 0)
		return false;

    return ::bind(_fd, reinterpret_cast<sockaddr*>(&default_addr), sizeof(default_addr)) == 0;
}

bool Socket::listen(int backlog) {
    return ::listen(_fd, backlog) == 0;
}

Socket Socket::accept() {
    sockaddr_in clientAddr{};
    socklen_t len = sizeof(clientAddr);
    int clientSock = ::accept(_fd, reinterpret_cast<sockaddr*>(&clientAddr), &len);
    if (clientSock < 0) {
        return Socket();
    }

    Socket client;
    client._fd = clientSock;
    return client;
}

bool Socket::connect(std::string_view ip, uint16_t port, sockaddr_in* addr) {
    sockaddr_in default_addr{};
    
    if (addr) default_addr = *addr;
    default_addr.sin_family = AF_INET;
    default_addr.sin_port = htons(port);

    if (::inet_pton(AF_INET, ip.data(), &default_addr.sin_addr) <= 0) return false;

    return ::connect(_fd, reinterpret_cast<sockaddr*>(&default_addr), sizeof(default_addr)) == 0;
}

bool Socket::send(std::string_view data) const {
    size_t totalSent = 0;
    while (totalSent < data.size()) {
        ssize_t sent = ::send(_fd, data.data() + totalSent, data.size() - totalSent, 0);
        if (sent <= 0) {
            if (errno == EINTR) continue;
            return false;
        }
        totalSent += sent;
    }
    return true;
}

std::string Socket::receive(size_t size) const {
    std::string buffer(size, '\0');
    size_t totalReceived = 0;

    while (totalReceived < size) {
        ssize_t bytes = ::recv(_fd, buffer.data() + totalReceived, size - totalReceived, 0);
        if (bytes == 0) return "";
        if (bytes < 0) {
            if (errno == EINTR) continue;
            if (errno == EAGAIN || errno == EWOULDBLOCK) break;
            return "";
        }
        totalReceived += bytes;
    }

    buffer.resize(totalReceived);
    return buffer;
}


void Socket::close() {
    if (isValid()) {
        ::close(_fd);
        _fd = -1;
    }
}

bool Socket::isValid() const {
    return _fd != -1;
}

int Socket::getFd() const {
    return _fd;
}
