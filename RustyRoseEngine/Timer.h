#pragma once
#include "Script.h"
#include <ctime>

class Timer
{
public:
	Timer();

	void pause();
	void resume();
	void reset();
	bool isPause();
	Script::Time elapsed();
	void setTimerToTime(Script::Time time);
	void setTimerSpeed(int speed);

private:
	int _speed;
	bool _pause;
	clock_t _previous_time;
	clock_t _elapsed_time;
};

