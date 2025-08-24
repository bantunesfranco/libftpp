/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   thread_safe_queue_test.cpp                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: bfranco <bfranco@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/25 18:51:49 by bfranco       #+#    #+#                 */
/*   Updated: 2025/08/24 12:45:19 by bfranco       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <thread>
#include "libftpp.hpp"

void assert(bool condition, const std::string& message) {
    if (!condition) throw std::runtime_error(message);
}

void test_empty_pop_throws() {
    ThreadSafeQueue<int> queue;

    std::string err;

    try {
        queue.pop_front();
    } catch (const std::out_of_range& e) {
        err = e.what();
    }
    assert(err == "Queue is empty", "pop_front() on empty queue did not throw the expected exception");

    err.clear();

    // pop_back on empty
    try {
        queue.pop_back();
    } catch (const std::out_of_range& e) {
        err = e.what();
    }
    assert(err == "Queue is empty", "pop_back() on empty queue did not throw the expected exception");
}

void test_push_and_pop_single_thread() {
    ThreadSafeQueue<int> queue;

    queue.push_back(10);
    queue.push_front(20);
    queue.push_back(30);

    int val = queue.pop_front();
	std::cout << "Popped front value: " << val << std::endl;
    assert(val == 20, "pop_front() did not return expected value");

    val = queue.pop_back();
	std::cout << "Popped back value: " << val << std::endl;
    assert(val == 30, "pop_back() did not return expected value");

    val = queue.pop_front();
	std::cout << "Popped front value: " << val << std::endl;
    assert(val == 10, "pop_front() did not return expected value");
}

void test_threaded_push_pop() {
    ThreadSafeQueue<int> queue;

    auto producer = [&queue]() {
        for (int i = 0; i < 100; ++i) {
            if (i % 2 == 0)
                queue.push_back(i);
            else
                queue.push_front(i);
        }
    };

    auto consumer = [&queue]() {
        int count = 0;
        while (count < 100) {
            try {
                if (count % 2 == 0)
                    queue.pop_front();
                else
                    queue.pop_back();
                ++count;
            } catch (const std::out_of_range&) {
                std::this_thread::yield();
            }
        }
    };

    Thread t1("t1", producer);
    Thread t2("t2", consumer);

    t1.start(); t2.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    t1.stop(); t2.stop();

    try {
        queue.pop_front();
        assert(false, "Queue should be empty after all pops");
    } catch (const std::out_of_range& e) {
        assert(std::string(e.what()) == "Queue is empty", "Expected 'Queue is empty' after all pops");
    }
}

int main() {
	test_empty_pop_throws();
	test_push_and_pop_single_thread();
	test_threaded_push_pop();
}
