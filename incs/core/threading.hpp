/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   threading.hpp                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: bfranco <bfranco@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/25 14:05:10 by bfranco       #+#    #+#                 */
/*   Updated: 2025/07/27 11:14:00 by bfranco       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef THREADS_HPP
#define THREADS_HPP

#include <thread>
#include <string>
#include <functional>
#include <condition_variable>
#include "threading/thread_safe_queue.hpp"
#include "threading/thread_safe_iostream.hpp"

class Thread
{
	private:
		bool _start;
		std::function<void()> _functionToExecute;
		std::mutex _mutex;
		std::condition_variable _condition;
		std::thread _thread;

	public:
		static thread_local std::string _name;	
		static const std::string& getName();
		
		Thread(std::string name, std::function<void()> functToExecute);
		~Thread();
		Thread(const Thread&) = delete;
		Thread(Thread&&) = delete;

		void start();
		void stop();
};

#endif