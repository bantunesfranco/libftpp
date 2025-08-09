/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   thread_safe_iostream.cpp                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: bfranco <bfranco@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/27 09:46:50 by bfranco       #+#    #+#                 */
/*   Updated: 2025/07/30 11:31:26 by bfranco       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "core/threading.hpp"

std::mutex ThreadSafeIOStream::_mutex;
thread_local std::string ThreadSafeIOStream::_prefix = "[Main] ";
thread_local ThreadSafeIOStream ThreadSafeIOStream::threadSafeCout;

ThreadSafeIOStream::ThreadSafeIOStream() : _buffer() {
	_setPrefix(Thread::getCurrentThreadName());
}

ThreadSafeIOStream& ThreadSafeIOStream::operator<<(std::ostream& (*manip)(std::ostream&)) {
	if (manip == static_cast<std::ostream& (*)(std::ostream&)>(std::endl)) {	
		_flushLine();
	} else {
		_buffer << manip;
	}
	return *this;
}

void ThreadSafeIOStream::_setPrefix(const std::string& prefix) {
	_prefix = "[" + prefix + "] ";
}

void ThreadSafeIOStream::_flushLine() {
	std::lock_guard<std::mutex> lock(_mutex);
	std::cout << _prefix << _buffer.str() << std::endl;
	std::cout.flush();
	_buffer.str("");
	_buffer.clear();
}