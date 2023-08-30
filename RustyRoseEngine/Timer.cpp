#include "Timer.h"

Timer::Timer()
{
	this->_start_time = clock();
	this->_pause = false;
	this->_pause_time = elapsed();
	this->_speed = 1;
}

void Timer::pause()
{
	this->_pause_time = this->elapsed();
	this->_pause = true;
}

void Timer::resume()
{
	int paused_duration_ms = this->elapsed().millisecond - this->_pause_time.millisecond +
		(this->elapsed().second - this->_pause_time.second) * 1000 +
		(this->elapsed().minute - this->_pause_time.minute) * 60 * 1000;

	this->_start_time += static_cast<clock_t>(paused_duration_ms * CLOCKS_PER_SEC / 1000.0);

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

	elapsed_time_ms *= this->_speed;
	
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

void Timer::setTimerSpeed(int speed)
{
	this->_speed = speed;
}
