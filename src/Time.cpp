#include "Time.hpp"

Time::Time(void) {
	_oldTime = glfwGetTime();
	_deltaTime = 0.0;
}

void Time::step(void) {
	double time = glfwGetTime();
	_deltaTime = time - _oldTime;
	_oldTime = time;
}

void Time::reset(void) {
	glfwSetTime(0.0);
	_oldTime = 0.0;
}

double Time::total(void) {
	return glfwGetTime();
}

double Time::delta(void) {
	return _deltaTime;
}
