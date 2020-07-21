#pragma once
#include <chrono>
#include <thread>
#include <iostream>

class DebugTimer
{
private:
public:

	std::chrono::time_point<std::chrono::steady_clock> start, end;
	std::chrono::duration<float> duration;
	
	DebugTimer()
	{
		start = std::chrono::high_resolution_clock::now();
	}
	~DebugTimer()
	{
		end = std::chrono::high_resolution_clock::now();
		duration = end - start;

		float ms = duration.count() * 1000.0f;
		std::cout << "Timer took " << ms << " ms" << "\n";
	}

};

