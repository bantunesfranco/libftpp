/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   state_machine.hpp                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: bfranco <bfranco@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/24 20:50:07 by bfranco       #+#    #+#                 */
/*   Updated: 2025/07/25 13:09:11 by bfranco       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef STATE_MACHINE_HPP
#define STATE_MACHINE_HPP

#include <unordered_set>
#include <functional>

template <typename TState>
class Observer;

namespace std {
	template <typename TState>
    struct hash<std::pair<TState, TState>> {
        std::size_t operator()(const std::pair<TState, TState>& p) const noexcept {
            std::size_t h1 = std::hash<TState>{}(p.first);
            std::size_t h2 = std::hash<TState>{}(p.second);
            return h1 ^ (h2 << 1);
        }
    };
}

template <typename TState>
class StateMachine {
	private:
		Observer<std::pair<TState, TState>> _transitions;
		Observer<TState> _actions;
		std::unordered_set<TState> _states;
		TState _currentState;
		TState _previousState;
		bool _init = false;
	
	public:
		StateMachine() = default;
		~StateMachine() = default;
		StateMachine(const StateMachine& other) = default;
		StateMachine& operator=(const StateMachine& other) = default;

		void setInitialState(const TState& state);
		void addState(const TState& state);
		void addTransition(const TState& startState, const TState& finalState, const std::function<void()>& lambda);
		void addAction(const TState& state, const std::function<void()>& lambda);
		void transitionTo(const TState& state);
		void update();
};

template <typename TState>
void StateMachine<TState>::setInitialState(const TState& state) {
	if (_init)
		throw std::runtime_error("State machine already initialized");
	_currentState = state;
	_previousState = state;
	_init = true;
}

template <typename TState>
void StateMachine<TState>::addState(const TState& state) {
	if (_states.find(state) != _states.end())
		throw std::runtime_error("State already exists");
	_states.insert(state);
}

template <typename TState>
void StateMachine<TState>::addTransition(const TState& startState, const TState& finalState, const std::function<void()>& lambda) {
	if (_states.find(startState) == _states.end() || _states.find(finalState) == _states.end())
		throw std::runtime_error("State not found");
	_transitions.subscribe(std::make_pair(startState, finalState), lambda);
}

template <typename TState>
void StateMachine<TState>::addAction(const TState& state, const std::function<void()>& lambda) {
	if (_states.find(state) == _states.end())
		throw std::runtime_error("State not found");
	_actions.subscribe(state, lambda);
}

template <typename TState>
void StateMachine<TState>::transitionTo(const TState& state) {
	if (!_init)
		throw std::runtime_error("State machine not initialized");
	if (_states.find(state) == _states.end())
		throw std::runtime_error("State not found");

	_previousState = _currentState;
	_currentState = state;
	_transitions.notify(std::make_pair(_previousState, _currentState));
}

template <typename TState>
void StateMachine<TState>::update() {
	if (!_init)
		throw std::runtime_error("State machine not initialized");
	std::cout << "Current state: " << _currentState << std::endl;
	_actions.notify(_currentState);
}

#endif