/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   chronometer_test.cpp                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: bfranco <bfranco@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/27 13:44:31 by bfranco       #+#    #+#                 */
/*   Updated: 2025/08/27 13:53:22 by bfranco       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libftpp.hpp"
#include <iostream>
#include <thread>

int main() {
    std::cout << "=== Chronometer Test ===\n";

    Chronometer chrono(TimeUnit::Milliseconds);

    chrono.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(400));
    chrono.split();

    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    chrono.split();

    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    chrono.stop();

    std::cout << "Final elapsed: " << chrono.elapsed() << " ms\n";

    const auto& records = chrono.getRecords();
    std::cout << "Recorded splits:\n";
    for (size_t i = 0; i < records.size(); ++i) {
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(records[i]).count();
        std::cout << "Split " << i + 1 << ": " << ms << " ms\n";
    }

    chrono.reset(TimeUnit::Seconds);
    chrono.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(1200));
    chrono.stop();
    std::cout << "After reset, elapsed: " << chrono.elapsed() << " s\n";

    return 0;
}
