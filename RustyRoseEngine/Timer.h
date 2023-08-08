#pragma once
#include "Script.h"
#include <ctime>

class Timer
{
public:
	Timer();

	void reset();
	Script::Time elapsed();
	void setTimerToTime(Script::Time time);

private:
	clock_t start_time;
};

