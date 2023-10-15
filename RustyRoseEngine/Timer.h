#pragma once
#include "SharedUtils.h"

class Timer
{
public:
	struct Time
	{
		unsigned int minutes = 0;
		unsigned int seconds = 0;
		unsigned int milliseconds = 0;

		bool operator<=(const Time& other) const {
			int thisTotalMilliseconds = minutes * 60 * 1000 + seconds * 1000 + milliseconds;
			int otherTotalMilliseconds = other.minutes * 60 * 1000 + other.seconds * 1000 + other.milliseconds;
			return thisTotalMilliseconds <= otherTotalMilliseconds;
		}

		bool operator>=(const Time& other) const {
			int thisTotalMilliseconds = minutes * 60 * 1000 + seconds * 1000 + milliseconds;
			int otherTotalMilliseconds = other.minutes * 60 * 1000 + other.seconds * 1000 + other.milliseconds;
			return thisTotalMilliseconds >= otherTotalMilliseconds;
		}

		Time operator+(const Time& other) const {
			Time result;

			result.minutes = minutes + other.minutes;
			result.seconds = seconds + other.seconds;
			result.milliseconds = milliseconds + other.milliseconds;

			if (result.milliseconds >= 1000) {
				unsigned int carrySeconds = result.milliseconds / 1000;
				result.seconds += carrySeconds;
				result.milliseconds %= 1000;
			}

			if (result.seconds >= 60) {
				unsigned int carryMinutes = result.seconds / 60;
				result.minutes += carryMinutes;
				result.seconds %= 60;
			}

			return result;
		}

		Time operator-(const Time& other) const {
			Time result;

			int thisTotalMilliseconds = minutes * 60 * 1000 + seconds * 1000 + milliseconds;
			int otherTotalMilliseconds = other.minutes * 60 * 1000 + other.seconds * 1000 + other.milliseconds;
			int diffMilliseconds = thisTotalMilliseconds - otherTotalMilliseconds;

			if (diffMilliseconds >= 0) {
				result.minutes = diffMilliseconds / (60 * 1000);
				diffMilliseconds %= (60 * 1000);
				result.seconds = diffMilliseconds / 1000;
				result.milliseconds = diffMilliseconds % 1000;
			}
			else {
				result.minutes = result.seconds = result.milliseconds = 0;
			}

			return result;
		}

		static Time* millisecondsToTime(uint32_t milliseconds) {
			Time* time = new Time;

			time->minutes = milliseconds / (60 * 1000);
			milliseconds %= (60 * 1000);
			time->seconds = milliseconds / 1000;
			milliseconds %= 1000;
			time->milliseconds = milliseconds;

			return time;
		}

		std::string getString() {
			std::string time;
			time += std::to_string(this->minutes);
			time += ":";
			time += std::to_string(this->seconds);
			time += ":";
			time += std::to_string(this->milliseconds);
			return time;
		}

	};


	Timer();

	void pause();
	void resume();
	void reset();

	bool isPause();
	Time elapsed();

	void setTimerToTime(Time time);
	void setTimerSpeed(int level);
	void setTimerSpeedUp();
	void setTimerSpeedDown();

	double getSpeed();

private:
	double _speedLevels[5] = { 1, 2, 4, 16, 32 };
	int _currentSpeedLevelIndex = 0;

	bool _pause;
	clock_t _previous_time;
	clock_t _elapsed_time;
};

