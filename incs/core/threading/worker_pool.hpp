/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   worker_pool.hpp                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: bfranco <bfranco@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/27 21:25:10 by bfranco       #+#    #+#                 */
/*   Updated: 2025/07/30 12:26:29 by bfranco       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef WORKER_POOL_HPP
#define WORKER_POOL_HPP

// #include <functional>
// #include <map>

// #include "core/data_structures/pool.hpp"

// class Thread;

// class WorkerPool {
// 	private:
// 		struct FunctionComparator {
// 			bool operator()(const std::function<void()>& a, const std::function<void()>& b) const {
// 				return &a < &b;
// 			}
// 		};
		
// 		Pool<Thread> _workers;
// 		std::map<const std::string&, const std::function<void()>&, FunctionComparator> _tasks;

// 		WorkerPool(const WorkerPool& other) = delete;
// 		WorkerPool& operator=(const WorkerPool& other) = delete;

// 	public:
// 		WorkerPool(size_t size = 10);
// 		~WorkerPool() = default;
		
// 		void addJob(const std::string& name, const std::function<void()>& jobToExecute);
// 		void removeTask(const std::string& name);
// };

#endif