#include "Script.h"

Script::Script(std::string path)
{
	this->_status = false;
	this->_path = path;
	
	std::fstream file;
	file.open(this->_path.c_str(), std::ios::in | std::ios::binary);

	if (!file.good()) {
		printf("unable to load script file: %s\n", this->_path.c_str());
		return;
	}

	if (!this->_loadScript(&file)) {
		printf("something went wrong while load script '%s' - loading stop\n", this->_path.c_str());
	}

	this->_status = true;
	file.close();
}

std::string Script::getPath()
{
	return this->_path;
}

std::vector<Script::Event*> Script::getEvents()
{
	return this->_events;
}

bool Script::isStatusGood()
{
	return this->_status;
}

void Script::free() 
{
	for (int i = 0; i < this->_events.size(); i++) {
		delete(this->_events[i]);
	}
}

bool Script::_loadScript(std::fstream* file)
{
	uint32_t buff32; uint16_t buff16; uint8_t buff8; char buffS[255];
	this->_wipeCharArr(buffS, 255);
	file->read(&buffS[0], 0x8);

	if (strcmp(buffS, "RoseScri") != 0) {
		printf("Error - Header is missing: %s\n", this->_path.c_str());
		return false;
	}

	file->read(reinterpret_cast<char*>(&buff32), sizeof(buff32));
	int size = buff32;

	while (file->tellg() != size) {
		file->read(reinterpret_cast<char*>(&buff16), sizeof(buff16));
		//printf("offset: 0x%X, value: 0x%X\n", ((int)file->tellg())-2, buff16);
		switch (buff16) {
		case 0xCC01:
			this->_loadSkipFrame(file);
			break;

		case 0xCC02:
			this->_loadPlayBgm(file);
			break;

		case 0xCC03:
			this->_loadCreateBG(file);
			break;

		case 0xCC04:
			this->_loadPrintText(file);
			break;

		case 0xCC05:
			this->_loadPlayVoice(file);
			break;

		case 0xCC06:
			this->_loadPlaySe(file);
			break;

		case 0xCC07:
			this->_loadNext(file);
			break;

		case 0xCC08:
			this->_loadPlayMovie(file);
			break;

		case 0xCC09:
			this->_loadBlackFade(file);
			break;

		case 0xCC0A:
			this->_loadWhiteFade(file);
			break;

		case 0xCC0B:
			this->_loadSetSELECT(file);
			break;

		case 0xCC0C:
			this->_loadEndBGM(file);
			break;

		case 0xCC0D:
			this->_loadEndRoll(file);
			break;

		case 0xCC0E:
			this->_loadMoveSom(file);
			break;

		default:
			printf("ERROR - unknown command: 0x%X\n", buff16);
			return false;
			break;
		}
	}

	return true;
}

bool Script::_loadSkipFrame(std::fstream* file)
{
	uint32_t buff32;

	Event* event = new Event;
	event->action = 0xCC01;
	file->read(reinterpret_cast<char*>(&buff32), sizeof(buff32));
	event->start = this->_miliSecToTime(buff32);

	this->_events.push_back(event);
	return true;
}

bool Script::_loadPlayBgm(std::fstream* file)
{
	uint32_t buff32; uint16_t buff16; char buffS[255];
	this->_wipeCharArr(buffS, 255);
	
	Event* event = new Event;
	event->action = 0xCC02;
	file->read(reinterpret_cast<char*>(&buff32), sizeof(buff32));
	event->start = this->_miliSecToTime(buff32);
	file->read(reinterpret_cast<char*>(&buff16), sizeof(buff16));
	file->read(&buffS[0], buff16);
	event->data = std::string(buffS);
	file->read(reinterpret_cast<char*>(&buff32), sizeof(buff32));
	event->end = this->_miliSecToTime(buff32);

	this->_events.push_back(event);
	return true;
}

bool Script::_loadCreateBG(std::fstream* file) 
{
	uint32_t buff32; uint16_t buff16; char buffS[255];
	this->_wipeCharArr(buffS, 255);

	Event* event = new Event;
	event->action = 0xCC03;
	file->read(reinterpret_cast<char*>(&buff32), sizeof(buff32));
	event->start = this->_miliSecToTime(buff32);
	file->read(reinterpret_cast<char*>(&buff16), sizeof(buff16));
	event->bgshit = (buff16 == 0xBBBB) ? "BGS" : "0xAAAA??";
	file->read(reinterpret_cast<char*>(&buff16), sizeof(buff16));
	file->read(&buffS[0], buff16);
	event->data = std::string(buffS);
	file->read(reinterpret_cast<char*>(&buff32), sizeof(buff32));
	event->end = this->_miliSecToTime(buff32);

	this->_events.push_back(event);
	return true;
}

bool Script::_loadPrintText(std::fstream* file)
{
	uint32_t buff32; uint16_t buff16; char buffS[255];
	this->_wipeCharArr(buffS, 255);

	Event* event = new Event;
	event->action = 0xCC04;
	file->read(reinterpret_cast<char*>(&buff32), sizeof(buff32));
	event->start = this->_miliSecToTime(buff32);
	file->read(reinterpret_cast<char*>(&buff16), sizeof(buff16));
	file->read(&buffS[0], buff16);
	event->name = std::string(buffS);
	file->read(reinterpret_cast<char*>(&buff16), sizeof(buff16));
	this->_wipeCharArr(buffS, 255);
	file->read(&buffS[0], buff16);
	event->data = std::string(buffS);
	file->read(reinterpret_cast<char*>(&buff32), sizeof(buff32));
	event->end = this->_miliSecToTime(buff32);

	this->_events.push_back(event);
	return true;
}

bool Script::_loadPlayVoice(std::fstream* file)
{
	uint32_t buff32; uint16_t buff16; uint8_t buff8; char buffS[255];
	this->_wipeCharArr(buffS, 255);

	Event* event = new Event;
	event->action = 0xCC05;
	file->read(reinterpret_cast<char*>(&buff32), sizeof(buff32));
	event->start = this->_miliSecToTime(buff32);
	file->read(reinterpret_cast<char*>(&buff16), sizeof(buff16));
	file->read(&buffS[0], buff16);
	event->data = std::string(buffS);
	file->read(reinterpret_cast<char*>(&buff8), sizeof(buff8));
	event->isMale = (buff8 == 1) ? true : false;
	this->_wipeCharArr(buffS, 255);
	file->read(&buffS[0], 3);
	event->shortName = std::string(buffS);
	file->read(reinterpret_cast<char*>(&buff32), sizeof(buff32));
	event->end = this->_miliSecToTime(buff32);

	this->_events.push_back(event);
	return true;
}

bool Script::_loadPlaySe(std::fstream* file)
{
	uint32_t buff32; uint16_t buff16; uint8_t buff8; char buffS[255];
	this->_wipeCharArr(buffS, 255);

	Event* event = new Event;
	event->action = 0xCC06;
	file->read(reinterpret_cast<char*>(&buff32), sizeof(buff32));
	event->start = this->_miliSecToTime(buff32);
	file->read(reinterpret_cast<char*>(&buff8), sizeof(buff8));
	event->layer = buff8;
	file->read(reinterpret_cast<char*>(&buff16), sizeof(buff16));
	file->read(&buffS[0], buff16);
	event->data = std::string(buffS);
	file->read(reinterpret_cast<char*>(&buff32), sizeof(buff32));
	event->end = this->_miliSecToTime(buff32);

	this->_events.push_back(event);
	return true;
}

bool Script::_loadNext(std::fstream* file)
{
	uint32_t buff32;

	Event* event = new Event;
	event->action = 0xCC07;
	file->read(reinterpret_cast<char*>(&buff32), sizeof(buff32));
	event->start = this->_miliSecToTime(buff32);

	this->_events.push_back(event);
	return true;
}

bool Script::_loadPlayMovie(std::fstream* file)
{
	uint32_t buff32; uint16_t buff16; uint8_t buff8; char buffS[255];
	this->_wipeCharArr(buffS, 255);

	Event* event = new Event;
	event->action = 0xCC08;
	file->read(reinterpret_cast<char*>(&buff32), sizeof(buff32));
	event->start = this->_miliSecToTime(buff32);
	file->read(reinterpret_cast<char*>(&buff16), sizeof(buff16));
	file->read(&buffS[0], buff16);
	event->data = std::string(buffS);
	file->read(reinterpret_cast<char*>(&buff8), sizeof(buff8));
	event->layer = buff8;
	file->read(reinterpret_cast<char*>(&buff32), sizeof(buff32));
	event->end = this->_miliSecToTime(buff32);

	this->_events.push_back(event);
	return true;
}

bool Script::_loadBlackFade(std::fstream* file)
{
	uint32_t buff32; uint8_t buff8;

	Event* event = new Event;
	event->action = 0xCC09;
	file->read(reinterpret_cast<char*>(&buff32), sizeof(buff32));
	event->start = this->_miliSecToTime(buff32);
	file->read(reinterpret_cast<char*>(&buff8), sizeof(buff8));
	event->fade = (buff8 == 0) ? false : true;
	file->read(reinterpret_cast<char*>(&buff32), sizeof(buff32));
	event->end = this->_miliSecToTime(buff32);

	this->_events.push_back(event);
	return true;
}

bool Script::_loadWhiteFade(std::fstream* file)
{
	uint32_t buff32; uint8_t buff8;

	Event* event = new Event;
	event->action = 0xCC0A;
	file->read(reinterpret_cast<char*>(&buff32), sizeof(buff32));
	event->start = this->_miliSecToTime(buff32);
	file->read(reinterpret_cast<char*>(&buff8), sizeof(buff8));
	event->fade = (buff8 == 0) ? false : true;
	file->read(reinterpret_cast<char*>(&buff32), sizeof(buff32));
	event->end = this->_miliSecToTime(buff32);

	this->_events.push_back(event);
	return true;
}

bool Script::_loadSetSELECT(std::fstream* file)
{
	uint32_t buff32; uint16_t buff16; char buffS[255];
	this->_wipeCharArr(buffS, 255);

	Event* event = new Event;
	event->action = 0xCC0B;
	file->read(reinterpret_cast<char*>(&buff32), sizeof(buff32));
	event->start = this->_miliSecToTime(buff32);
	file->read(reinterpret_cast<char*>(&buff16), sizeof(buff16));
	file->read(&buffS[0], buff16);
	event->data = std::string(buffS);
	file->read(reinterpret_cast<char*>(&buff16), sizeof(buff16));
	if (buff16 != 0x0000) {
		this->_wipeCharArr(buffS, 255);
		file->read(&buffS[0], buff16);
		event->data = event->data + std::string(buffS);
	}
	file->read(reinterpret_cast<char*>(&buff32), sizeof(buff32));
	event->end = this->_miliSecToTime(buff32);

	this->_events.push_back(event);
	return true;
}

bool Script::_loadEndBGM(std::fstream* file)
{
	uint32_t buff32; uint16_t buff16; char buffS[255];
	this->_wipeCharArr(buffS, 255);

	Event* event = new Event;
	event->action = 0xCC0C;
	file->read(reinterpret_cast<char*>(&buff32), sizeof(buff32));
	event->start = this->_miliSecToTime(buff32);
	file->read(reinterpret_cast<char*>(&buff16), sizeof(buff16));
	file->read(&buffS[0], buff16);
	event->data = std::string(buffS);
	file->read(reinterpret_cast<char*>(&buff32), sizeof(buff32));
	event->end = this->_miliSecToTime(buff32);

	this->_events.push_back(event);
	return true;
}

bool Script::_loadEndRoll(std::fstream* file)
{
	uint32_t buff32; uint16_t buff16; char buffS[255];
	this->_wipeCharArr(buffS, 255);

	Event* event = new Event;
	event->action = 0xCC0D;
	file->read(reinterpret_cast<char*>(&buff32), sizeof(buff32));
	event->start = this->_miliSecToTime(buff32);
	file->read(reinterpret_cast<char*>(&buff16), sizeof(buff16));
	file->read(&buffS[0], buff16);
	event->data = std::string(buffS);
	file->read(reinterpret_cast<char*>(&buff32), sizeof(buff32));
	event->end = this->_miliSecToTime(buff32);

	this->_events.push_back(event);
	return true;
}

bool Script::_loadMoveSom(std::fstream* file)
{
	uint32_t buff32; uint8_t buff8;

	Event* event = new Event;
	event->action = 0xCC0D;
	file->read(reinterpret_cast<char*>(&buff32), sizeof(buff32));
	event->start = this->_miliSecToTime(buff32);
	file->read(reinterpret_cast<char*>(&buff8), sizeof(buff8));
	event->layer = buff8;
	file->read(reinterpret_cast<char*>(&buff32), sizeof(buff32));
	event->end = this->_miliSecToTime(buff32);

	this->_events.push_back(event);
	return true;
}

Script::Time* Script::_miliSecToTime(uint32_t milisec)
{
	Time* time = new Time;
	
	time->minute = milisec / (60 * 1000);
	milisec %= (60 * 1000);
	time->second = milisec / 1000;
	milisec %= 1000;
	time->millisecond = milisec;

	return time;
}

void Script::_wipeCharArr(char* arr, int size)
{
	memset(arr, 0x00, size);
}