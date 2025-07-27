/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   thread.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: bfranco <bfranco@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/25 19:26:20 by bfranco       #+#    #+#                 */
/*   Updated: 2025/07/27 11:24:33 by bfranco       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libftpp.hpp"

thread_local std::string Thread::_name = "Main";

Thread::Thread(std::string name, std::function<void()> functToExecute):
	_start(false), _functionToExecute(functToExecute), _mutex(), _condition() {
		_thread = std::thread([this, name]() {
		_name = name;
		std::unique_lock<std::mutex> lock(_mutex);
		_condition.wait(lock, [this]() { return _start; });
		lock.unlock();
		_functionToExecute();
	});
}

Thread::~Thread() {
	{
		std::lock_guard<std::mutex> lock(_mutex);
		_start = true;
	}
	_condition.notify_all();

	if (_thread.joinable()) {
		_thread.join();
	}
}

void Thread::start() {
	std::unique_lock<std::mutex> lock(_mutex);
	if (_start) {
		throw std::runtime_error("Thread already started");
	}
	_start = true;
	_condition.notify_all();
}

void Thread::stop() {
	std::unique_lock<std::mutex> lock(_mutex);
	if (!_start) {
		throw std::runtime_error("Thread not started");
	}
	if (_thread.joinable()) {
		lock.unlock();
		_thread.join();
	}
}

const std::string& Thread::getName() {
	return _name;
}