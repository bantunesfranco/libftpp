/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   persistent_worker.hpp                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: bfranco <bfranco@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/30 11:44:20 by bfranco       #+#    #+#                 */
/*   Updated: 2025/08/24 19:22:01 by bfranco       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PERSISTENT_WORKER_HPP
#define PERSISTENT_WORKER_HPP

#include <map>
#include <string>
#include <functional>
#include <atomic>
#include <condition_variable>
#include "core/threading/thread.hpp"

class PersistentWorker {
    private:
        Thread _thread;
        std::map<std::string, std::function<void()>> _tasks;
        std::mutex _mutex;
        std::atomic<bool> _running{true};
        std::condition_variable _cv;

        void run();

    public:
        PersistentWorker();
        ~PersistentWorker();

        void addTask(const std::string& name, const std::function<void()>& jobToExecute);
        void removeTask(const std::string& name);
        void stop();
};

#endif