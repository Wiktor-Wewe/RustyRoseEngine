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
	};

	struct Event 
	{
		int action = 0;
		Time* start;
		std::string bgshit;
		int layer = 0;
		std::string name;
		std::string data;
		bool isMale = false;
		std::string shortName;
		Time* end;
	};
	
	Script(std::string path);

private:
	std::string _path;
	std::vector<Event*> events;

	Event* _lineToEvent(std::string line);
	int _codeAction(std::string action);
	Time* _stringToTime(std::string time);
};

