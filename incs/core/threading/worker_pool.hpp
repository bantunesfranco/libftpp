/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   worker_pool.hpp                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: bfranco <bfranco@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/27 21:25:10 by bfranco       #+#    #+#                 */
/*   Updated: 2025/08/24 19:06:49 by bfranco       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef WORKER_POOL_HPP
#define WORKER_POOL_HPP

#include <vector>
#include <queue>
#include <functional>
#include <atomic>
#include <condition_variable>
#include "core/threading.hpp"

class WorkerPool {
	private:
		std::vector<std::thread> _workers;
		std::queue<std::function<void()>> _jobs;
		std::mutex _mutex;
		std::condition_variable _cv;
		std::atomic<bool> _stop{false};
		
		void _workerLoop();

	public:
		struct IJobs {
			virtual ~IJobs() = default;
			virtual void execute() = 0;
		};

		WorkerPool(std::size_t numWorkers);
		~WorkerPool();

		void addJob(const std::function<void()>& jobToExecute);
		void addJob(std::unique_ptr<IJobs> job);
};

#endif