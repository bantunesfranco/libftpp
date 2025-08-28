/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   socket.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: bfranco <bfranco@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/30 20:11:11 by bfranco       #+#    #+#                 */
/*   Updated: 2025/08/29 01:11:45 by bfranco       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <string_view>
#include <sys/socket.h>
#include <arpa/inet.h>
class Socket {
	private:
	int _fd;
	public:
	Socket();
		~Socket();
		Socket(Socket&& other);
		Socket& operator=(Socket&& other);
		Socket(const Socket&) = delete;
		Socket& operator=(const Socket&) = delete;

		bool create();
		bool bind(std::string_view ip, uint16_t port, sockaddr_in* addr = nullptr);
		bool listen(int backlog = 5);
		Socket accept();
		bool connect(std::string_view ip, uint16_t port, sockaddr_in* addr = nullptr);

		bool send(std::string_view data) const;
		std::string receive(size_t size = 1024) const;
		
		void close();
		bool isValid() const;
		int getFd() const;
};

#endif