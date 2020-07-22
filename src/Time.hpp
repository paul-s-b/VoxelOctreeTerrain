#ifndef TIME_H_
#define TIME_H_

#include <glew.h>
#include <glfw3.h>

// Time utility class
class Time {
	double _oldTime;
	double _deltaTime;
public:
	Time(void);

	//! Reset the total time to zero, see Time.Total
	void reset(void);

	//! Step time forward, measure delta time
	void step(void);

	//! Time since the creation of the Time object or the last call to Time.Reset
	double total(void);

	//! Time since the last call to Time.Step
	double delta(void);
};

#endif // TIME_H_
