#include "Timer.h"

Timer::Timer()
{
	this->start_time = clock();
}

void Timer::reset()
{
	this->start_time = clock();
}

Script::Time Timer::elapsed()
{
	clock_t end_time = clock();
	double elapsed_time_ms = (end_time - start_time) * 1000.0 / CLOCKS_PER_SEC;
	
	int total_seconds = static_cast<int>(elapsed_time_ms / 1000);
	int miliseconds = static_cast<int>(elapsed_time_ms) % 1000;
	int minutes = total_seconds / 60;
	int seconds = total_seconds % 60;

	Script::Time time;
	time.millisecond = miliseconds;
	time.second = seconds;
	time.minute = minutes;

	return time;
}
