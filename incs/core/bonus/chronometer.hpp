/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   chronometer.hpp                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: bfranco <bfranco@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/27 13:25:13 by bfranco       #+#    #+#                 */
/*   Updated: 2025/08/27 13:35:57 by bfranco       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHRONOMETER_HPP
#define CHRONOMETER_HPP

#include <chrono>
#include <vector>
#include "timer.hpp"

class Chronometer {
	private:
		using Clock = std::chrono::steady_clock;

		Clock::time_point _start;
		std::chrono::nanoseconds _elapsed{0};
		TimeUnit _timeUnit;
		bool _running{false};

		std::vector<std::chrono::nanoseconds> _records;

	public:
		Chronometer(TimeUnit unit = TimeUnit::Milliseconds);
		~Chronometer() = default;

		void start();
		void split();
		void stop();
		void reset();
		void reset(TimeUnit unit);


		long long elapsed(TimeUnit unit) const;
		long long elapsed() const;
		bool isRunning() const { return _running; }

		const std::vector<std::chrono::nanoseconds>& getRecords() const { return _records; }
};


#endif