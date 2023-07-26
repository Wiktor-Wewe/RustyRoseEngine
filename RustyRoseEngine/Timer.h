#pragma once
#include "Script.h"
#include <ctime>

class Timer
{
public:
	Timer();

	void reset();
	Script::Time elapsed();

private:
	clock_t start_time;
};

