/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   persistent_worker.cpp                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: bfranco <bfranco@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/24 19:14:58 by bfranco       #+#    #+#                 */
/*   Updated: 2025/08/24 19:17:11 by bfranco       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "core/threading/persistent_worker.hpp"
#include <iostream>

PersistentWorker::PersistentWorker() : _thread("PersistentWorker", [this]() { this->run(); }) { _thread.start(); }

PersistentWorker::~PersistentWorker() { stop(); }

void PersistentWorker::addTask(const std::string& name, const std::function<void()>& jobToExecute) {
	{
		std::lock_guard<std::mutex> lock(_mutex);
		_tasks[name] = jobToExecute;
	}
	_cv.notify_all();
}

void PersistentWorker::removeTask(const std::string& name) {
	std::lock_guard<std::mutex> lock(_mutex);
	_tasks.erase(name);
}

void PersistentWorker::stop() {
	_running.store(false);
	_cv.notify_all();
}

void PersistentWorker::run() {
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
				std::cerr << "Error executing task: " << name << std::endl;
			}
		}
	}
}