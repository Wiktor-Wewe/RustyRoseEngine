#pragma once
#include "SharedUtils.h"
#include "Timer.h"

class Script
{
public:
	enum EventType {
		SkipFRAME	= 0xCC01,
		PlayBgm		= 0xCC02,
		CreateBG	= 0xCC03,
		PrintText	= 0xCC04,
		PlayVoice	= 0xCC05,
		PlaySe		= 0xCC06,
		Next		= 0xCC07,
		PlayMovie	= 0xCC08,
		BlackFade	= 0xCC09,
		WhiteFade	= 0xCC0A,
		SetSELECT	= 0xCC0B,
		EndBGM		= 0xCC0C,
		EndRoll		= 0xCC0D,
		MoveSom		= 0xCC0E
	};

	struct Event
	{
		uint16_t type = 0x0000; // EventType
		Timer::Time* start = nullptr;
		std::string bgshit;
		int layer = 0;
		std::string name;
		std::string data;
		bool isEroge = false;
		bool isMale = false;
		bool fade = false; // [OUT] = 0 | [IN] = 1
		std::string shortName;
		Timer::Time* end = nullptr;

		~Event() {
			if (this->start) {
				delete this->start; // todo delete in time
			}
			if (this->end) {
				delete this->end;
			}
		}
	};

	struct EventsStateLists {
		std::vector<Script::Event*> toLoad;
		std::vector<Script::Event*> toStart;
		std::vector<Script::Event*> inProgress;

		~EventsStateLists() {
			for (auto& event : this->toLoad) {
				delete event;
			}

			for (auto& event : this->toStart) {
				delete event;
			}

			for (auto& event : this->inProgress) {
				delete event;
			}
		}
	};

	Script(std::string path);
	std::string getPath();
	std::vector<Event*> getEvents();
	bool isGood();

	~Script();

private:
	bool _status;
	std::string _path;
	std::vector<Event*> _events;
	Timer::Time* _time;

	bool _loadScript(std::fstream* file);

	void _loadSkipFrame(std::fstream* file);
	void _loadPlayBgm(std::fstream* file);
	void _loadCreateBG(std::fstream* file);
	void _loadPrintText(std::fstream* file);
	void _loadPlayVoice(std::fstream* file);
	void _loadPlaySe(std::fstream* file);
	void _loadNext(std::fstream* file);
	void _loadPlayMovie(std::fstream* file);
	void _loadBlackFade(std::fstream* file);
	void _loadWhiteFade(std::fstream* file);
	void _loadSetSELECT(std::fstream* file);
	void _loadEndBGM(std::fstream* file);
	void _loadEndRoll(std::fstream* file);
	void _loadMoveSom(std::fstream* file);
};

