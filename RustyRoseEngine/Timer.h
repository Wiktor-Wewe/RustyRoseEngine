#pragma once
#include "SharedUtils.h"

class Timer
{
public:
	struct Time
	{
		unsigned int minute = 0;
		unsigned int second = 0;
		unsigned int millisecond = 0;

		bool operator<=(const Time& other) const {
			int thisTotalMilliseconds = minute * 60 * 1000 + second * 1000 + millisecond;
			int otherTotalMilliseconds = other.minute * 60 * 1000 + other.second * 1000 + other.millisecond;
			return thisTotalMilliseconds <= otherTotalMilliseconds;
		}

		bool operator>=(const Time& other) const {
			int thisTotalMilliseconds = minute * 60 * 1000 + second * 1000 + millisecond;
			int otherTotalMilliseconds = other.minute * 60 * 1000 + other.second * 1000 + other.millisecond;
			return thisTotalMilliseconds >= otherTotalMilliseconds;
		}

		Time operator+(const Time& other) const {
			Time result;

			result.minute = minute + other.minute;
			result.second = second + other.second;
			result.millisecond = millisecond + other.millisecond;

			if (result.millisecond >= 1000) {
				unsigned int carrySeconds = result.millisecond / 1000;
				result.second += carrySeconds;
				result.millisecond %= 1000;
			}

			if (result.second >= 60) {
				unsigned int carryMinutes = result.second / 60;
				result.minute += carryMinutes;
				result.second %= 60;
			}

			return result;
		}

		Time operator-(const Time& other) const {
			Time result;

			int thisTotalMilliseconds = minute * 60 * 1000 + second * 1000 + millisecond;
			int otherTotalMilliseconds = other.minute * 60 * 1000 + other.second * 1000 + other.millisecond;
			int diffMilliseconds = thisTotalMilliseconds - otherTotalMilliseconds;

			if (diffMilliseconds >= 0) {
				result.minute = diffMilliseconds / (60 * 1000);
				diffMilliseconds %= (60 * 1000);
				result.second = diffMilliseconds / 1000;
				result.millisecond = diffMilliseconds % 1000;
			}
			else {
				result.minute = result.second = result.millisecond = 0;
			}

			return result;
		}

		std::string getString() {
			std::string time;
			time += std::to_string(this->minute);
			time += ":";
			time += std::to_string(this->second);
			time += ":";
			time += std::to_string(this->millisecond);
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
	void setTimerSpeed(int speed);

private:
	int _speed;
	bool _pause;
	clock_t _previous_time;
	clock_t _elapsed_time;
};

