/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   observable_value.hpp                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: bfranco <bfranco@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/26 22:17:28 by bfranco       #+#    #+#                 */
/*   Updated: 2025/08/26 22:44:45 by bfranco       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef OBSERVABLE_VALUE_HPP
#define OBSERVABLE_VALUE_HPP

#include <string>
#include "core/design_patterns/observer.hpp"

template<typename TType, typename TEvent = std::string>
class ObservableValue {
	private:
		TType _value;
		Observer<TEvent> _observer;
		TEvent _onChangedEvent;

	public:
		ObservableValue(const TType& value): _value(value), _observer(), _onChangedEvent("changed")
		{
			auto callback = [this]() { std::cout << "Value changed to: " << _value << std::endl; };
			_observer.subscribe(_onChangedEvent, callback);
		}

		~ObservableValue() {}

		void set(const TType& newValue) {
			if (_value != newValue) {
				_value = newValue;
				_observer.notify(_onChangedEvent);
			}
		}
		const TType& get() const { return _value; }
		
		ObservableValue& operator=(const TType& value) { set(value); return *this; }
		const TType& operator()() { return get(); }
};

#endif