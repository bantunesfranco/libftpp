/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   socket.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: bfranco <bfranco@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/30 20:11:11 by bfranco       #+#    #+#                 */
/*   Updated: 2025/07/30 20:19:38 by bfranco       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <string.h>
#include <string_view>

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
		bool bind(std::string_view ip, uint16_t port);
		bool listen(int backlog = 5);
		Socket accept();
		bool connect(std::string_view ip, uint16_t port);

		size_t send(std::string_view data) const;
		std::string receive(size_t size = 1024) const;
		
		void close();
		bool isValid() const;
		int getFd() const;
};

#endif