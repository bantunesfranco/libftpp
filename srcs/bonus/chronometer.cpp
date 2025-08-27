/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   chronometer.cpp                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: bfranco <bfranco@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/27 13:37:04 by bfranco       #+#    #+#                 */
/*   Updated: 2025/08/27 14:00:05 by bfranco       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "core/bonus/chronometer.hpp"

Chronometer::Chronometer(TimeUnit unit) : _timeUnit(unit) {}

void Chronometer::start() {
    if (!_running) {
        _running = true;
        _start = Clock::now();
    }
}

void Chronometer::split() {
    if (_running) {
        auto now = Clock::now();
        auto current = _elapsed + std::chrono::duration_cast<std::chrono::nanoseconds>(now - _start);
        _records.push_back(current);
    }
}

void Chronometer::stop() {
    if (_running) {
        auto now = Clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(now - _start);
        _elapsed += duration;
        _records.push_back(_elapsed);  // store cumulative time
        _running = false;
    }
}

void Chronometer::reset() {
    _elapsed = std::chrono::nanoseconds(0);
    _records.clear();
    _running = false;
}

void Chronometer::reset(TimeUnit unit) {
    reset();
    _timeUnit = unit;
}

long long Chronometer::elapsed(TimeUnit unit) const {
    std::chrono::nanoseconds total = _elapsed;
    if (_running) {
        total += std::chrono::duration_cast<std::chrono::nanoseconds>(Clock::now() - _start);
    }

    switch (unit) {
        case TimeUnit::Seconds:      return std::chrono::duration_cast<std::chrono::seconds>(total).count();
        case TimeUnit::Milliseconds: return std::chrono::duration_cast<std::chrono::milliseconds>(total).count();
        case TimeUnit::Microseconds: return std::chrono::duration_cast<std::chrono::microseconds>(total).count();
        case TimeUnit::Nanoseconds:  return total.count();
    }
    return -1;
}

long long Chronometer::elapsed() const {
    return elapsed(_timeUnit);
}
