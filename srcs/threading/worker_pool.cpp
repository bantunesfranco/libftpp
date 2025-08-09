/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   worker_pool.cpp                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: bfranco <bfranco@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/28 13:39:44 by bfranco       #+#    #+#                 */
/*   Updated: 2025/07/$28 13:39:45 by bfranco       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "core/threading.hpp"

using TSIO = ThreadSafeIOStream;

// WorkerPool::WorkerPool(size_t size): _workers(size), _tasks() {};

// void WorkerPool::addJob(const std::string& name, const std::function<void()>& jobToExecute) {
// 	if (_tasks.find(name) != _tasks.end())
// 		throw std::runtime_error("Error: Task with name \"" + name + "\" already exists");

// 	auto tasks = _tasks;
// 	auto task = [&, name, tasks, jobToExecute]() {
// 		while (tasks.find(name) != tasks.end())
// 			TSIO::threadSafeCout << "Executing " << name << std::endl;
// 				jobToExecute();
// 		TSIO::threadSafeCout << "Done" << std::endl;
// 	};
	

// }

// void WorkerPool::removeTask(const std::string& name) {
// 	for (auto& worker : _workers.getPool())
// 	{
// 		if (worker.getName() == name)
// 		{
// 			worker.stop()
// 		}
// 	}

// }
