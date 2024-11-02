/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   observer.hpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: bfranco <bfranco@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/31 21:22:46 by bfranco       #+#    #+#                 */
/*   Updated: 2024/11/01 11:45:11 by bfranco       ########   odam.nl         */
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
			bool operator()(const std::reference_wrapper<const std::function<void()>>& a,
							const std::reference_wrapper<const std::function<void()>>& b) const {
				return &a.get() < &b.get();
			}
		};

		std::unordered_map<TEvent, std::set<std::reference_wrapper<const std::function<void()>>, FunctionComparator>>  _observables;

	public:
		Observer() = default;
		~Observer() = default;
		Observer(const Observer& other);
		Observer& operator=(const Observer& other);

		void subscribe(const TEvent& event, const std::function<void()>& lambda);
		void notify(const TEvent& event);
		std::unordered_map<TEvent, std::set<std::reference_wrapper<const std::function<void()>>, FunctionComparator>>  getObservables(void) const { return _observables; };
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
		task.get()();
}

template<typename TEvent>
void	Observer<TEvent>::subscribe(const TEvent& event, const std::function<void()>& lambda)
{
	_observables[event].insert(std::cref(lambda));
}

#endif