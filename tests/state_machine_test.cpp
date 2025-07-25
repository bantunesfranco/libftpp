#include "libftpp.hpp"
#include <iostream>
#include <string>

bool didInit = false, didS1 = false, didS2 = false, didS3 = false, didCompleted = false;
bool tInitS1 = false, tS1S2 = false, tS2S3 = false, tS3Completed = false;

typedef enum State {
    INIT,
    STAGE1,
    STAGE2,
    STAGE3,
    COMPLETED,
    UNKNOWN
} State;

void assert(bool condition, const std::string& message) {
    if (!condition) throw std::runtime_error(message);
}

void test() {
	std::cout << "Test function called" << std::endl;
}

int main() {
    StateMachine<State> sm;

	sm.addState(INIT);
    sm.addState(STAGE1);
    sm.addState(STAGE2);
    sm.addState(STAGE3);
    sm.addState(COMPLETED);

    try {
        sm.addState(INIT);
    } catch (const std::runtime_error& e) {
        assert(std::string(e.what()) == "State already exists", "Expected exception for duplicate state");
    }

    try {
        sm.update();
	} catch (const std::runtime_error& e) {
		assert(std::string(e.what()) == "State machine not initialized", "Expected exception for update before init");
	}

	sm.setInitialState(INIT);

    try {
        sm.setInitialState(INIT);
	} catch (const std::runtime_error& e) {
		assert(std::string(e.what()) == "State machine already initialized", "Expected exception for double initialiazation");
	}

    try {
        sm.addTransition(INIT, static_cast<State>(UNKNOWN), [](){});
	} catch (const std::runtime_error& e) {
		assert(std::string(e.what()) == "State not found", "Expected exception for addTransition with unknown state");
	}

    try {
        sm.addAction(static_cast<State>(UNKNOWN), [](){});
	} catch (const std::runtime_error& e) {
		assert(std::string(e.what()) == "State not found", "Expected exception for addTransition with unknown state");
	}

    try {
        sm.transitionTo(static_cast<State>(UNKNOWN));
	} catch (const std::runtime_error& e) {
		assert(std::string(e.what()) == "State not found", "Expected exception for addTransition with unknown state");
	}

	sm.addAction(INIT, test);
    sm.addAction(INIT, std::function<void()>([&](){ didInit = true; std::cout << "INIT action called" << std::endl; }));
    sm.addAction(STAGE1, std::function<void()>([&](){ didS1 = true; std::cout << "STAGE1 action called" << std::endl; }));
    sm.addAction(STAGE2, std::function<void()>([&](){ didS2 = true; std::cout << "STAGE2 action called" << std::endl; }));
    sm.addAction(STAGE3, std::function<void()>([&](){ didS3 = true; std::cout << "STAGE3 action called" << std::endl; }));
    sm.addAction(COMPLETED, std::function<void()>([&](){ didCompleted = true; std::cout << "COMPLETED action called" << std::endl; }));

    sm.addTransition(INIT, STAGE1, std::function<void()>([&](){ tInitS1 = true; }));
    sm.addTransition(STAGE1, STAGE2, std::function<void()>([&](){ tS1S2 = true; }));
    sm.addTransition(STAGE2, STAGE3, std::function<void()>([&](){ tS2S3 = true; }));
    sm.addTransition(STAGE3, COMPLETED, std::function<void()>([&](){ tS3Completed = true; }));

	sm.update();
	assert(didInit, "INIT action should have been called");

    sm.transitionTo(STAGE1);
    assert(tInitS1, "Transition INIT->STAGE1 should have been called");
    sm.update();
    assert(didS1, "STAGE1 action should have been called");

    sm.transitionTo(STAGE2);
    assert(tS1S2, "Transition STAGE1->STAGE2 should have been called");
    sm.update();
    assert(didS2, "STAGE2 action should have been called");

    sm.transitionTo(STAGE3);
    assert(tS2S3, "Transition STAGE2->STAGE3 should have been called");
    sm.update();
    assert(didS3, "STAGE3 action should have been called");

    sm.transitionTo(COMPLETED);
    assert(tS3Completed, "Transition STAGE3->COMPLETED should have been called");
    sm.update();
    assert(didCompleted, "COMPLETED action should have been called");

    return 0;
}