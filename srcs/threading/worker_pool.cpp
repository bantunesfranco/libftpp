/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   worker_pool.cpp                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: bfranco <bfranco@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/24 14:20:00 by bfranco       #+#    #+#                 */
/*   Updated: 2025/08/24 19:06:16 by bfranco       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "core/threading/worker_pool.hpp"
#include <iostream>

WorkerPool::WorkerPool(std::size_t numWorkers) {
    for (std::size_t i = 0; i < numWorkers; ++i) {
        _workers.emplace_back([this]() { _workerLoop(); });
    }
}

WorkerPool::~WorkerPool() {
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _stop.store(true, std::memory_order_release);
    }
    _cv.notify_all();

    for (auto& t : _workers) {
        if (t.joinable()) t.join();
    }
}

void WorkerPool::addJob(std::unique_ptr<IJobs> jobToExecute) {
    auto jobShared = std::shared_ptr<IJobs>(std::move(jobToExecute));
    addJob([jobShared]() {
        try {
            jobShared->execute();
        } catch (const std::exception& e) {
            std::cerr << "Exception in job: " << e.what() << std::endl;
        }
    });
}

void WorkerPool::addJob(const std::function<void()>& jobToExecute) {
    {
        std::lock_guard<std::mutex> lock(_mutex);
        if (_stop.load(std::memory_order_acquire)) {
            throw std::runtime_error("Cannot add jobs to a stopped WorkerPool");
        }
        _jobs.push(jobToExecute);
    }
    _cv.notify_one();
}

void WorkerPool::_workerLoop() {
    while (true) {
        std::function<void()> job;

        {
            std::unique_lock<std::mutex> lock(_mutex);
            _cv.wait(lock, [this]() { return _stop.load() || !_jobs.empty(); });

            if (_stop.load() && _jobs.empty()) return;

            job = std::move(_jobs.front());
            _jobs.pop();
        }

        try {
            job();
        } catch (const std::exception& e) {
            std::cerr << "WorkerPool caught exception: " << e.what() << std::endl;
        } catch (...) {
            std::cerr << "WorkerPool caught unknown exception" << std::endl;
        }
    }
}
