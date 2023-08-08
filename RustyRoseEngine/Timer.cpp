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

void Timer::setTimerToTime(Script::Time time)
{
	int total_milliseconds = time.millisecond + time.second * 1000 + time.minute * 60 * 1000;

	clock_t current_ticks = clock();
	clock_t desired_ticks = current_ticks - static_cast<clock_t>(total_milliseconds * CLOCKS_PER_SEC / 1000.0);

	this->start_time = desired_ticks;
}
