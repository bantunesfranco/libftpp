/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   timer_test.cpp                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: bfranco <bfranco@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/27 13:15:34 by bfranco       #+#    #+#                 */
/*   Updated: 2025/08/27 13:24:39 by bfranco       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libftpp.hpp"

void assert(bool condition, const std::string& message) {
	if (!condition) throw std::runtime_error(message);
}

void testPassiveMode() {
    std::cout << "\n=== Passive Mode Test ===\n";
    Timer timer(3, TimeUnit::Seconds);

    while (!timer.isExpired()) {
        std::cout << "Time left: " << timer.timeLeft(TimeUnit::Seconds) << " sec\n";
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
	assert(timer.timeLeft() == 0, "timer did not expire");
    std::cout << "Passive: Timer expired!\n";

    timer.setDuration(3000, TimeUnit::Microseconds);

    while (!timer.isExpired()) {
        std::cout << "Time left: " << timer.timeLeft(TimeUnit::Microseconds) << " sec\n";
        std::this_thread::sleep_for(std::chrono::microseconds(500));
    }
	assert(timer.timeLeft() == 0, "timer did not expire");
    std::cout << "Passive: Timer expired!\n";
}

void testActiveMode() {
    std::cout << "\n=== Active Mode Test ===\n";
    Timer timer(2, TimeUnit::Seconds);

    timer.subscribe([]() {
        std::cout << "Active: Timer expired! Callback fired.\n";
    });

    timer.startAsync();

    std::this_thread::sleep_for(std::chrono::seconds(3));
	assert(timer.timeLeft() == 0, "timer did not expire");

	timer.setDuration(2000, TimeUnit::Microseconds);

    timer.startAsync();

    std::this_thread::sleep_for(std::chrono::microseconds(3000));
	assert(timer.timeLeft() == 0, "timer did not expire");
}

int main() {
    testPassiveMode();
    testActiveMode();
    return 0;
}
