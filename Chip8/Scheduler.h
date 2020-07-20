#pragma once
#include <chrono>

class Scheduler {
public:
	Scheduler(int Hertz);

	double getDelay();
	void setDelay(int Hertz);

	std::chrono::steady_clock::time_point getTime();
	void setTime(std::chrono::steady_clock::time_point timeValue);

	double duration(std::chrono::steady_clock::time_point time);

	bool run(std::chrono::steady_clock::time_point time);
	bool run(Scheduler scheduler);

private:
	std::chrono::steady_clock::time_point time;
	double delay;
};

