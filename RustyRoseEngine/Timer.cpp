#include "Timer.h"

Timer::Timer()
{
	this->_previous_time = clock();
	this->_pause = false;
	this->_elapsed_time = clock() - this->_previous_time;
}

void Timer::pause()
{
	this->_pause = true;
}

void Timer::resume()
{
	this->_previous_time = clock();
	this->_pause = false;
}

void Timer::reset()
{
	this->_previous_time = clock();
	this->_elapsed_time = clock() - this->_previous_time;
	this->_pause = false;
}

bool Timer::isPause()
{
	return this->_pause;
}

Timer::Time Timer::elapsed()
{
	if (!this->_pause) {
		this->_elapsed_time += (clock() - this->_previous_time) * this->_speedLevels[this->_currentSpeedLevelIndex];
		this->_previous_time = clock();
	}

	double elapsed_time_ms = this->_elapsed_time * 1000.0 / CLOCKS_PER_SEC;
	
	int total_seconds = static_cast<int>(elapsed_time_ms / 1000);
	int miliseconds = static_cast<int>(elapsed_time_ms) % 1000;
	int minutes = total_seconds / 60;
	int seconds = total_seconds % 60;

	Timer::Time time;
	time.milliseconds = miliseconds;
	time.seconds = seconds;
	time.minutes = minutes;

	return time;
}

void Timer::setTimerToTime(Timer::Time time)
{
	int total_milliseconds = time.milliseconds + time.seconds * 1000 + time.minutes * 60 * 1000;

	clock_t desired_ticks = static_cast<clock_t>(total_milliseconds * CLOCKS_PER_SEC / 1000.0);

	this->_previous_time = clock();
	this->_elapsed_time = desired_ticks;
}

void Timer::setTimerSpeed(int level)
{
	if (level > 4) level = 4;
	if (level < 0) level = 0;
	this->_currentSpeedLevelIndex = level;
}

void Timer::setTimerSpeedUp()
{
	int newLevel = this->_currentSpeedLevelIndex + 1;
	this->setTimerSpeed(newLevel);
}

void Timer::setTimerSpeedDown()
{
	int newLevel = this->_currentSpeedLevelIndex - 1;
	this->setTimerSpeed(newLevel);
}

double Timer::getSpeed()
{
	return this->_speedLevels[this->_currentSpeedLevelIndex];
}
