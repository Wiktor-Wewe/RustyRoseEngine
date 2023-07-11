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
		uint16_t action = 0;
		Time* start;
		std::string bgshit;
		int layer = 0;
		std::string name;
		std::string data;
		bool isEroge = false;
		bool isMale = false;
		bool fade = false; // [OUT] = 0 | [IN] = 1
		std::string shortName;
		Time* end;
	};
	
	Script(std::string path);

private:
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

