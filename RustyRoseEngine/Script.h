#pragma once
#include <fstream>
#include <string>
#include <vector>
class Script
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

		std::string getString(){
			std::string time;
			time += std::to_string(this->minute);
			time += ":";
			time += std::to_string(this->second);
			time += ":";
			time += std::to_string(this->millisecond);
			return time;
		}

	};

	struct Event 
	{
		uint16_t action = 0;
		Time* start = nullptr;
		std::string bgshit;
		int layer = 0;
		std::string name;
		std::string data;
		bool isEroge = false;
		bool isMale = false;
		bool fade = false; // [OUT] = 0 | [IN] = 1
		std::string shortName;
		Time* end = nullptr;
	};
	
	Script(std::string path);
	std::string getPath();
	std::vector<Event*> getEvents();
	bool isStatusGood();
	
	void free();

private:
	bool _status;
	std::string _path;
	std::vector<Event*> _events;

	bool _loadScript(std::fstream* file);
	bool _loadSkipFrame(std::fstream* file);
	bool _loadPlayBgm(std::fstream* file);
	bool _loadCreateBG(std::fstream* file);
	bool _loadPrintText(std::fstream* file);
	bool _loadPlayVoice(std::fstream* file);
	bool _loadPlaySe(std::fstream* file);
	bool _loadNext(std::fstream* file);
	bool _loadPlayMovie(std::fstream* file);
	bool _loadBlackFade(std::fstream* file);
	bool _loadWhiteFade(std::fstream* file);
	bool _loadSetSELECT(std::fstream* file);
	bool _loadEndBGM(std::fstream* file);
	bool _loadEndRoll(std::fstream* file);
	bool _loadMoveSom(std::fstream* file);

	Time* _miliSecToTime(uint32_t milisec);
	void _wipeCharArr(char* arr, int size);
};

