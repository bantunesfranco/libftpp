/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   timer.hpp                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: bfranco <bfranco@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/27 12:58:01 by bfranco       #+#    #+#                 */
/*   Updated: 2025/08/27 13:30:03 by bfranco       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef TIMER_HPP
#define TIMER_HPP

#include <chrono>
#include <thread>
#include <atomic>
#include <vector>
#include <functional>
#include <mutex>

enum class TimeUnit {
    Seconds,
    Milliseconds,
    Microseconds,
    Nanoseconds
};

class Timer {
    private:
        using Clock = std::chrono::steady_clock;

        Clock::time_point _start;
        std::chrono::nanoseconds _duration;
        std::atomic<bool> _running{false};
        std::thread _worker;

        std::vector<std::function<void()>> _callbacks;
        std::mutex _cbMutex;

    public:
        Timer(long long value = 0, TimeUnit unit = TimeUnit::Milliseconds);
        ~Timer();

        void setDuration(long long value, TimeUnit unit);
        void reset();

        // ---- Passive mode ----
        bool isExpired() const;
        long long timeLeft(TimeUnit unit = TimeUnit::Milliseconds) const;

        // ---- Active mode ----
        void subscribe(const std::function<void()>& cb);
        void startAsync();
        void stopAsync();
};


#endif


