#include "Timer.h"

Timer::Timer()
{
	this->_start_time = clock();
	this->_pause = false;
	this->_pause_time = elapsed();
}

void Timer::pause()
{
	this->_pause_time = this->elapsed();
	this->_pause = true;
}

void Timer::resume()
{
	this->setTimerToTime(this->_pause_time);
	this->_pause = false;
}

void Timer::reset()
{
	this->_start_time = clock();
	this->_pause_time = elapsed();
	this->_pause = false;
}

bool Timer::isPause()
{
	return this->_pause;
}

Script::Time Timer::elapsed()
{
	if (this->_pause) {
		return this->_pause_time;
	}

	clock_t end_time = clock();
	double elapsed_time_ms = (end_time - this->_start_time) * 1000.0 / CLOCKS_PER_SEC;
	
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

	this->_start_time = desired_ticks;
}
