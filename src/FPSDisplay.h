#ifndef FPSDISPLAY_H_
#define FPSDISPLAY_H_

#include "Time.h"
#include "Window.h"
#include "Text.h"
#include <iomanip>

class	FPSDisplay {
	int _index;
	double _times[60];
	double _timer;
	Time _clock;
	Text* _text = nullptr;

public:

	FPSDisplay();
	~FPSDisplay();
	void render(Window&);
};

#endif // FPSDISPLAY_H_