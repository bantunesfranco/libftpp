/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   socket.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: bfranco <bfranco@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/30 11:08:32 by bfranco       #+#    #+#                 */
/*   Updated: 2025/07/30 23:23:32 by bfranco       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "core/network/socket.hpp"

#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
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
    _fd = ::socket(AF_INET, SOCK_STREAM, 0);
    return _fd != -1;
}

bool Socket::bind(std::string_view ip, uint16_t port) {
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    if (::inet_pton(AF_INET, ip.data(), &addr.sin_addr) <= 0)
		return false;

    return ::bind(_fd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == 0;
}

bool Socket::listen(int backlog) {
    return ::listen(_fd, backlog) == 0;
}

Socket Socket::accept() {
    sockaddr_in clientAddr{};
    socklen_t len = sizeof(clientAddr);
    int clientSock = ::accept(_fd, reinterpret_cast<sockaddr*>(&clientAddr), &len);

    Socket client;
    client._fd = clientSock;
    return client;
}

bool Socket::connect(std::string_view ip, uint16_t port) {
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    if (::inet_pton(AF_INET, ip.data(), &addr.sin_addr) <= 0) return false;

    return ::connect(_fd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == 0;
}

size_t Socket::send(std::string_view data) const {
    return ::send(_fd, data.data(), data.size(), 0);
}

std::string Socket::receive(size_t size) const {
    std::string buffer(size, '\0');
    size_t bytes = ::recv(_fd, &buffer[0], size, 0);
    if (bytes <= 0) return {};
    buffer.resize(static_cast<size_t>(bytes));
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
