/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   observer_test.cpp                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: bfranco <bfranco@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/31 22:26:59 by bfranco       #+#    #+#                 */
/*   Updated: 2024/11/01 11:44:54 by bfranco       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libftpp.hpp"

typedef enum Event
{
	CONNECT,
	DISCONNECT,
	MESSAGE,
} Event;

void connect() { std::cout << "Event: connect" << std::endl; }
void disconnect() { std::cout << "Event: disconnect" << std::endl; }
void message() { std::cout << "Event: message" << std::endl; }
void lol() { std::cout << "Event: lol" << std::endl; }

int main()
{
	Observer<Event> obs;

	std::vector<std::function<void()>> funcs;
	funcs.push_back(connect);
	funcs.push_back(disconnect);
	funcs.push_back(message);
	funcs.push_back(lol);

	obs.subscribe(CONNECT, funcs[0]);
	obs.subscribe(DISCONNECT, funcs[1]);
	obs.subscribe(MESSAGE, funcs[2]);
	obs.subscribe(MESSAGE, funcs[3]);

	obs.notify(CONNECT);
	obs.notify(DISCONNECT);
	obs.notify(MESSAGE);
}