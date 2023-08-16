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

private:
	bool _pause;
	clock_t _start_time;
	Script::Time _pause_time;
};

