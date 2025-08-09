/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   persistent_worker.hpp                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: bfranco <bfranco@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/30 11:44:20 by bfranco       #+#    #+#                 */
/*   Updated: 2025/07/30 11:54:53 by bfranco       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include 

class Thread;
#include "thread.hpp" // Your Thread wrapper header

class PersistentWorker {
private:
    Thread _thread;
    std::map<std::string, std::function<void()>> _tasks;
    std::mutex _mutex;
    std::atomic<bool> _running{true};
    std::condition_variable _cv;

    void run() {
        while (_running.load()) {
            std::map<std::string, std::function<void()>> tasksCopy;

            {
                std::unique_lock<std::mutex> lock(_mutex);
                _cv.wait(lock, [this]() { return !_tasks.empty() || !_running.load(); });
                if (!_running.load()) break;
                tasksCopy = _tasks;
            }

            for (auto& [name, task] : tasksCopy) {
                if (!_running.load()) break;
                try {
                    task();
                } catch (...) {
                    // handle exceptions
                }
            }
        }
    }

public:
    PersistentWorker()
        : _thread("PersistentWorker", [this]() { this->run()    ; }) 
    {
        _thread.start();
    }

    ~PersistentWorker() {
        stop();
        // _thread will join in its destructor
    }

    void addTask(const std::string& name, const std::function<void()>& jobToExecute) {
        {
            std::lock_guard<std::mutex> lock(_mutex);
            _tasks[name] = jobToExecute;
        }
        _cv.notify_all();
    }

    void removeTask(const std::string& name) {
        std::lock_guard<std::mutex> lock(_mutex);
        _tasks.erase(name);
    }

    void stop() {
        _running.store(false);
        _cv.notify_all();
    }
};
