/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   observer.hpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: bfranco <bfranco@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/31 21:22:46 by bfranco       #+#    #+#                 */
/*   Updated: 2025/07/25 13:40:06 by bfranco       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef OBSERVER_HPP
#define OBSERVER_HPP

#include <unordered_map>
#include <functional>
#include <set>

template<typename TEvent>
class Observer
{
	private:
	struct FunctionComparator {
		bool operator()(const std::function<void()>& a, const std::function<void()>& b) const {
			return &a < &b;
		}
	};
	
	std::unordered_map<TEvent, std::set<std::function<void()>, FunctionComparator>>  _observables;
	
	public:
		Observer() = default;
		~Observer() = default;
		Observer(const Observer& other);
		Observer& operator=(const Observer& other);

		void subscribe(const TEvent& event, const std::function<void()>& lambda);
		void notify(const TEvent& event); 
		auto getObservables(void) const { return _observables; };
};

template<typename TEvent>
Observer<TEvent>::Observer(const Observer& other) : _observables(other._observables) {}

template<typename TEvent>
Observer<TEvent>& Observer<TEvent>::operator=(const Observer& other) {
	if (this != &other) {
		_observables = other._observables;
	}
	return *this;
}

template<typename TEvent>
void	Observer<TEvent>::notify(const TEvent& event)
{
	auto&	tasks = _observables[event];
	for (auto& task: tasks)
		task();
}

template<typename TEvent>
void	Observer<TEvent>::subscribe(const TEvent& event, const std::function<void()>& lambda)
{
	_observables[event].insert(lambda);
}

#endif