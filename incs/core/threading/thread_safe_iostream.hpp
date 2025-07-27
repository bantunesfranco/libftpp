/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   thread_safe_iostream.hpp                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: bfranco <bfranco@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/25 20:28:11 by bfranco       #+#    #+#                 */
/*   Updated: 2025/07/27 20:45:52 by bfranco       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef THREAD_SAFE_IOSTREAM_HPP
#define THREAD_SAFE_IOSTREAM_HPP

#include <iostream>
#include <sstream>
#include <mutex>
#include <string>
#include <thread>

class ThreadSafeIOStream {
    private:
        static std::mutex _mutex;
        static thread_local std::string _prefix;

        std::ostringstream _buffer;

        void _setPrefix(const std::string& prefix);
        void _flushLine();

    public:
        // Static instance for thread-local usage, similar to std::cout
        static thread_local ThreadSafeIOStream threadSafeCout;
        
        ThreadSafeIOStream();
        ~ThreadSafeIOStream() = default;
        ThreadSafeIOStream(const ThreadSafeIOStream&) = delete;
        ThreadSafeIOStream& operator=(const ThreadSafeIOStream&) = delete;
        ThreadSafeIOStream& operator<<(std::ostream& (*manip)(std::ostream&));

        template <typename T>
        ThreadSafeIOStream& operator<<(const T& value) {
            _buffer << value;
            return *this;
        }
};

#endif