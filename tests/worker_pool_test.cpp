/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   worker_pool_test.cpp                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: bfranco <bfranco@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/24 14:35:00 by bfranco       #+#    #+#                 */
/*   Updated: 2025/08/24 18:52:58 by bfranco       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <stdexcept>
#include <atomic>
#include <thread>
#include <chrono>
#include "libftpp.hpp"

struct IncrementJob : public WorkerPool::IJobs {
	std::atomic<int>& counter;
	IncrementJob(std::atomic<int>& c) : counter(c) {}
	void execute() override {
		++counter;
	}
};

void assert(bool condition, const std::string& message) {
    if (!condition) throw std::runtime_error(message);
}

void test_IJobs_interface() {
    WorkerPool pool(4);

    std::atomic<int> counter{0};
    for (int i = 0; i < 100; ++i) {
        pool.addJob(std::make_unique<IncrementJob>(counter));
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    assert(counter == 100, "Counter should be 100 after 100 IncrementJobs");
}

void test_single_job_execution() {
    WorkerPool pool(2);

    std::atomic<bool> executed{false};
    pool.addJob([&]() {
        executed = true;
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    assert(executed.load(), "Job was not executed by WorkerPool");
}

void test_multiple_jobs_execution() {
    WorkerPool pool(4);

    std::atomic<int> counter{0};
    const int jobCount = 50;

    for (int i = 0; i < jobCount; ++i) {
        pool.addJob([&counter]() {
            counter.fetch_add(1, std::memory_order_relaxed);
        });
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    assert(counter.load() == jobCount, "Not all jobs were executed");
}

void test_exception_in_job() {
    WorkerPool pool(2);

    std::atomic<bool> executedAfter{false};

    pool.addJob([]() {
        throw std::runtime_error("Intentional failure");
    });

    pool.addJob([&]() {
        executedAfter = true;
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    assert(executedAfter.load(), "WorkerPool stopped after exception");
}

int main() {
	test_single_job_execution();
	test_multiple_jobs_execution();
	test_exception_in_job();
	test_IJobs_interface();
}
