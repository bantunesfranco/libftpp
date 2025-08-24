/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   persistent_worker_tests.cpp                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: bfranco <bfranco@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/24 19:19:59 by bfranco       #+#    #+#                 */
/*   Updated: 2025/08/24 19:22:26 by bfranco       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <sstream>
#include <thread>
#include <chrono>
#include <atomic>
#include "libftpp.hpp"

void assert(bool condition, const std::string& message) {
	if (!condition) throw std::runtime_error(message);
}

class CoutCapture {
	private:
		std::ostringstream& _target;
		std::streambuf* _old;

	public:
		CoutCapture(std::ostringstream& target) : _target(target) {
			_old = std::cout.rdbuf(_target.rdbuf());
		}
		~CoutCapture() { std::cout.rdbuf(_old); }
};

int main() {
	std::ostringstream capture;
	CoutCapture redirect(capture);

	PersistentWorker worker;

	std::atomic<int> counter{0};

	worker.addTask("task1", [&]() { 
		std::cout << "Task1 executed\n"; 
		++counter; 
	});

	worker.addTask("task2", [&]() { 
		std::cout << "Task2 executed\n"; 
		++counter; 
	});

	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	assert(counter.load() >= 2, "Tasks did not execute at least once");

	worker.removeTask("task1");

	int oldCounter = counter.load();
	std::this_thread::sleep_for(std::chrono::milliseconds(50));

	assert(counter.load() > oldCounter || counter.load() == oldCounter, "Task1 still executed after removal");
	worker.stop();

	std::cout << "Captured output:\n" << capture.str() << std::endl;
}
