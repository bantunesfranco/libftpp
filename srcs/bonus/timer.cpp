/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   timer.cpp                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: bfranco <bfranco@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/27 13:13:07 by bfranco       #+#    #+#                 */
/*   Updated: 2025/08/27 13:44:03 by bfranco       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "core/bonus/timer.hpp"

Timer::Timer(long long value, TimeUnit unit) { setDuration(value, unit); }

Timer::~Timer() { stopAsync(); }

void Timer::reset() { _start = Clock::now(); }

bool Timer::isExpired() const { return Clock::now() - _start >= _duration; }

void Timer::setDuration(long long value, TimeUnit unit) {
    switch (unit) {
        case TimeUnit::Seconds:      _duration = std::chrono::seconds(value); break;
        case TimeUnit::Milliseconds: _duration = std::chrono::milliseconds(value); break;
        case TimeUnit::Microseconds: _duration = std::chrono::microseconds(value); break;
        case TimeUnit::Nanoseconds:  _duration = std::chrono::nanoseconds(value); break;
    }
    _start = Clock::now();
}

long long Timer::timeLeft(TimeUnit unit) const {
    auto elapsed = Clock::now() - _start;
    auto remaining = _duration - std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed);
    if (remaining < std::chrono::nanoseconds(0)) remaining = std::chrono::nanoseconds(0);

    switch (unit) {
        case TimeUnit::Seconds:      return std::chrono::duration_cast<std::chrono::seconds>(remaining).count();
        case TimeUnit::Milliseconds: return std::chrono::duration_cast<std::chrono::milliseconds>(remaining).count();
        case TimeUnit::Microseconds: return std::chrono::duration_cast<std::chrono::microseconds>(remaining).count();
        case TimeUnit::Nanoseconds:  return remaining.count();
    }
    return -1;
}

void Timer::subscribe(const std::function<void()>& cb) {
    std::lock_guard<std::mutex> lock(_cbMutex);
    _callbacks.push_back(cb);
}

void Timer::startAsync() {
    stopAsync(); // ensure old worker is stopped
    _running = true;
    _start = Clock::now();

    _worker = std::thread([this]() {
        std::this_thread::sleep_for(_duration);
        if (_running) {
            std::lock_guard<std::mutex> lock(_cbMutex);
            for (auto& cb : _callbacks) cb();
        }
    });
}

void Timer::stopAsync() {
    _running = false;
    if (_worker.joinable()) _worker.join();
}
