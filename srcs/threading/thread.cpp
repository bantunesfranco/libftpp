/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   thread.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: bfranco <bfranco@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/25 19:26:20 by bfranco       #+#    #+#                 */
/*   Updated: 2025/07/30 12:29:19 by bfranco       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "core/threading.hpp"

thread_local std::string Thread::_threadName = "Main";

Thread::Thread(const std::string& name, std::function<void()> functToExecute)
    : _functionToExecute(std::move(functToExecute)), _name(name) 
{
    _thread = std::thread(&Thread::_threadEntry, this);
}

Thread::~Thread() {
    stop();
    if (_thread.joinable()) {
        _thread.join();
    }
}

Thread::Thread(Thread&& other) noexcept
    : _thread(std::move(other._thread)),
      _functionToExecute(std::move(other._functionToExecute)),
      _name(std::move(other._name)),
      _started(other._started.load()),
      _stopRequested(other._stopRequested.load())
{}

Thread& Thread::operator=(Thread&& other) noexcept {
    if (this != &other) {
        if (_thread.joinable()) {
            stop();
            _thread.join();
        }
        _thread = std::move(other._thread);
        _functionToExecute = std::move(other._functionToExecute);
        _name = std::move(other._name);
        _started = other._started.load();
        _stopRequested = other._stopRequested.load();
    }
    return *this;
}

void Thread::_threadEntry() {
    _threadName = _name;
    {
        std::unique_lock<std::mutex> lock(_startMutex);
        _startCondition.wait(lock, [this]() { return _started.load(); });
    }
    if (!_stopRequested.load()) {
        _functionToExecute();
    }
}

void Thread::start() {
    bool expected = false;
    if (!_started.compare_exchange_strong(expected, true)) {
        throw std::runtime_error("Thread already started");
    }
    _startCondition.notify_one();
}

void Thread::stop() {
    _stopRequested.store(true);
    {
        std::lock_guard<std::mutex> lock(_startMutex);
        _started = true;
    }
    _startCondition.notify_one();
    if (_thread.joinable()) {
        _thread.join();
    }
}

const std::string& Thread::getCurrentThreadName() {
    return _threadName;
}

const std::string& Thread::getName() const {
    return _name;
}