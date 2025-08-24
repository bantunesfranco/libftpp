/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   thread.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: bfranco <bfranco@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/24 19:18:42 by bfranco       #+#    #+#                 */
/*   Updated: 2025/08/24 19:19:05 by bfranco       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef THREAD_HPP
#define THREAD_HPP

#include <thread>
#include <functional>
#include <string>
#include <atomic>
#include <mutex>
#include <condition_variable>

class Thread {
    private:
        std::thread _thread;
        std::function<void()> _functionToExecute;
        std::string _name;

        std::mutex _startMutex;
        std::condition_variable _startCondition;
        std::atomic<bool> _started{false};
        std::atomic<bool> _stopRequested{false};

        void _threadEntry();

        static thread_local std::string _threadName;

    public:
        Thread(const std::string& name, std::function<void()> functToExecute);
        ~Thread();
        Thread(const Thread&) = delete;
        Thread& operator=(const Thread&) = delete;
        Thread(Thread&& other) noexcept;
        Thread& operator=(Thread&& other) noexcept;

        void start();
        void stop();
        const std::string& getName() const;

        static const std::string& getCurrentThreadName();
};

#endif