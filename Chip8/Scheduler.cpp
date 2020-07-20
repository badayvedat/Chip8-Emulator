#include "Scheduler.h"

/*
To convert hertz to milliseconds, first determine the duration or period of one vibration by dividing one second by the frequency in hertz. 
For a 500 Hz signal, the duration of one cycle is 1/500 or 0.002 seconds. Which is equal to 2 milliseconds.
*/

Scheduler::Scheduler(int Hertz) {
	time = std::chrono::steady_clock::now();
	delay = 1000 / Hertz;
}

double Scheduler::getDelay() {
	return delay;
}

void Scheduler::setDelay(int Hertz) {
	delay = 1000 / Hertz;
}

std::chrono::steady_clock::time_point Scheduler::getTime() {
	return time;
}

void Scheduler::setTime(std::chrono::steady_clock::time_point timeValue) {
	time = timeValue;
}

double Scheduler::duration(std::chrono::steady_clock::time_point lastTime) {
	return std::chrono::duration<double, std::chrono::milliseconds::period>(lastTime - time).count();
}

bool Scheduler::run(std::chrono::steady_clock::time_point time) {
	return delay < duration(time);
}

bool Scheduler::run(Scheduler scheduler) {
	return delay < duration(scheduler.getTime());
}
