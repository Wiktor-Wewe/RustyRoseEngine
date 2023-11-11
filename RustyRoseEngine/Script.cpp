#include "Script.h"

Script::Script(std::string path)
{
	this->_status = false;
	this->_path = path;
	this->_time = new Timer::Time();

	std::fstream file;
	file.open(this->_path.c_str(), std::ios::in | std::ios::binary);

	if (!file.good()) {
		RRE_LogError("unable to load script file: \n" + this->_path);
		return;
	}

	if (!this->_loadScript(&file)) {
		RRE_LogError("something went wrong while load script '" + this->_path + "' - loading stop");
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

bool Script::isGood()
{
	return this->_status;
}

Script::~Script()
{
	delete this->_time;

	for (auto& event : this->_events) {
		delete event;
	}
}

bool Script::_loadScript(std::fstream* file)
{
	uint32_t buff32; uint16_t buff16; uint8_t buff8; char buffS[255];
	memset(buffS, 0x00, 255);
	file->read(&buffS[0], 0x8);

	if (strcmp(buffS, "RoseScri") != 0) {
		RRE_LogError("Error - Header is missing: \n" + this->_path);
		return false;
	}

	file->read(reinterpret_cast<char*>(&buff32), sizeof(buff32));
	int size = buff32;

	while (file->tellg() != size) {
		file->read(reinterpret_cast<char*>(&buff16), sizeof(buff16));
		switch (buff16) {
		case EventType::SkipFRAME:
			this->_loadSkipFrame(file);
			break;

		case EventType::PlayBgm:
			this->_loadPlayBgm(file);
			break;

		case EventType::CreateBG:
			this->_loadCreateBG(file);
			break;

		case EventType::PrintText:
			this->_loadPrintText(file);
			break;

		case EventType::PlayVoice:
			this->_loadPlayVoice(file);
			break;

		case EventType::PlaySe:
			this->_loadPlaySe(file);
			break;

		case EventType::Next:
			this->_loadNext(file);
			break;

		case EventType::PlayMovie:
			this->_loadPlayMovie(file);
			break;

		case EventType::BlackFade:
			this->_loadBlackFade(file);
			break;

		case EventType::WhiteFade:
			this->_loadWhiteFade(file);
			break;

		case EventType::SetSELECT:
			this->_loadSetSELECT(file);
			break;

		case EventType::EndBGM:
			this->_loadEndBGM(file);
			break;

		case EventType::EndRoll:
			this->_loadEndRoll(file);
			break;

		case EventType::MoveSom:
			this->_loadMoveSom(file);
			break;

		default:
			printf("ERROR - unknown command: 0x%X\n", buff16); // todo log in RRE
			return false;
		}
	}

	return true;
}

void Script::_loadSkipFrame(std::fstream* file)
{
	uint32_t buff32;

	Event* event = new Event;
	event->type = EventType::SkipFRAME;
	file->read(reinterpret_cast<char*>(&buff32), sizeof(buff32));
	event->start = this->_time->millisecondsToTime(buff32);

	this->_events.push_back(event);
}

void Script::_loadPlayBgm(std::fstream* file)
{
	uint32_t buff32; uint16_t buff16; char buffS[255];
	memset(buffS, 0x00, 255);

	Event* event = new Event;
	event->type = EventType::PlayBgm;
	file->read(reinterpret_cast<char*>(&buff32), sizeof(buff32));
	event->start = this->_time->millisecondsToTime(buff32);
	file->read(reinterpret_cast<char*>(&buff16), sizeof(buff16));
	file->read(&buffS[0], buff16);
	event->data = std::string(buffS);
	file->read(reinterpret_cast<char*>(&buff32), sizeof(buff32));
	event->end = this->_time->millisecondsToTime(buff32);

	this->_events.push_back(event);
}

void Script::_loadCreateBG(std::fstream* file)
{
	uint32_t buff32; uint16_t buff16; char buffS[255];
	memset(buffS, 0x00, 255);

	Event* event = new Event;
	event->type = EventType::CreateBG;
	file->read(reinterpret_cast<char*>(&buff32), sizeof(buff32));
	event->start = this->_time->millisecondsToTime(buff32);
	file->read(reinterpret_cast<char*>(&buff16), sizeof(buff16));
	event->bgshit = (buff16 == 0xBBBB) ? "BGS" : "0xAAAA??";
	file->read(reinterpret_cast<char*>(&buff16), sizeof(buff16));
	file->read(&buffS[0], buff16);
	event->data = std::string(buffS);
	file->read(reinterpret_cast<char*>(&buff32), sizeof(buff32));
	event->end = this->_time->millisecondsToTime(buff32);

	this->_events.push_back(event);
}

void Script::_loadPrintText(std::fstream* file)
{
	uint32_t buff32; uint16_t buff16; char buffS[255];
	memset(buffS, 0x00, 255);

	Event* event = new Event;
	event->type = EventType::PrintText;
	file->read(reinterpret_cast<char*>(&buff32), sizeof(buff32));
	event->start = this->_time->millisecondsToTime(buff32);
	file->read(reinterpret_cast<char*>(&buff16), sizeof(buff16));
	file->read(&buffS[0], buff16);
	event->name = std::string(buffS);
	file->read(reinterpret_cast<char*>(&buff16), sizeof(buff16));
	memset(buffS, 0x00, 255);
	file->read(&buffS[0], buff16);
	event->data = std::string(buffS);
	file->read(reinterpret_cast<char*>(&buff32), sizeof(buff32));
	event->end = this->_time->millisecondsToTime(buff32);

	this->_events.push_back(event);
}

void Script::_loadPlayVoice(std::fstream* file)
{
	uint32_t buff32; uint16_t buff16; uint8_t buff8; char buffS[255];
	memset(buffS, 0x00, 255);

	Event* event = new Event;
	event->type = EventType::PlayVoice;
	file->read(reinterpret_cast<char*>(&buff32), sizeof(buff32));
	event->start = this->_time->millisecondsToTime(buff32);
	file->read(reinterpret_cast<char*>(&buff16), sizeof(buff16));
	file->read(&buffS[0], buff16);
	event->data = std::string(buffS);
	file->read(reinterpret_cast<char*>(&buff8), sizeof(buff8));
	event->isMale = (buff8 == 1) ? true : false;
	memset(buffS, 0x00, 255);
	file->read(&buffS[0], 3);
	event->shortName = std::string(buffS);
	file->read(reinterpret_cast<char*>(&buff32), sizeof(buff32));
	event->end = this->_time->millisecondsToTime(buff32);

	this->_events.push_back(event);
}

void Script::_loadPlaySe(std::fstream* file)
{
	uint32_t buff32; uint16_t buff16; uint8_t buff8; char buffS[255];
	memset(buffS, 0x00, 255);

	Event* event = new Event;
	event->type = EventType::PlaySe;
	file->read(reinterpret_cast<char*>(&buff32), sizeof(buff32));
	event->start = this->_time->millisecondsToTime(buff32);
	file->read(reinterpret_cast<char*>(&buff8), sizeof(buff8));
	event->layer = buff8;
	file->read(reinterpret_cast<char*>(&buff16), sizeof(buff16));
	file->read(&buffS[0], buff16);
	event->data = std::string(buffS);
	file->read(reinterpret_cast<char*>(&buff32), sizeof(buff32));
	event->end = this->_time->millisecondsToTime(buff32);

	this->_events.push_back(event);
}

void Script::_loadNext(std::fstream* file)
{
	uint32_t buff32;

	Event* event = new Event;
	event->type = EventType::Next;
	file->read(reinterpret_cast<char*>(&buff32), sizeof(buff32));
	event->start = this->_time->millisecondsToTime(buff32);

	this->_events.push_back(event);
}

void Script::_loadPlayMovie(std::fstream* file)
{
	uint32_t buff32; uint16_t buff16; uint8_t buff8; char buffS[255];
	memset(buffS, 0x00, 255);

	Event* event = new Event;
	event->type = EventType::PlayMovie;
	file->read(reinterpret_cast<char*>(&buff32), sizeof(buff32));
	event->start = this->_time->millisecondsToTime(buff32);
	file->read(reinterpret_cast<char*>(&buff16), sizeof(buff16));
	file->read(&buffS[0], buff16);
	event->data = std::string(buffS);
	file->read(reinterpret_cast<char*>(&buff8), sizeof(buff8));
	event->layer = buff8;
	file->read(reinterpret_cast<char*>(&buff32), sizeof(buff32));
	event->end = this->_time->millisecondsToTime(buff32);

	this->_events.push_back(event);
}

void Script::_loadBlackFade(std::fstream* file)
{
	uint32_t buff32; uint8_t buff8;

	Event* event = new Event;
	event->type = EventType::BlackFade;
	file->read(reinterpret_cast<char*>(&buff32), sizeof(buff32));
	event->start = this->_time->millisecondsToTime(buff32);
	file->read(reinterpret_cast<char*>(&buff8), sizeof(buff8));
	event->fade = (buff8 == 0) ? false : true;
	file->read(reinterpret_cast<char*>(&buff32), sizeof(buff32));
	event->end = this->_time->millisecondsToTime(buff32);

	this->_events.push_back(event);
}

void Script::_loadWhiteFade(std::fstream* file)
{
	uint32_t buff32; uint8_t buff8;

	Event* event = new Event;
	event->type = EventType::WhiteFade;
	file->read(reinterpret_cast<char*>(&buff32), sizeof(buff32));
	event->start = this->_time->millisecondsToTime(buff32);
	file->read(reinterpret_cast<char*>(&buff8), sizeof(buff8));
	event->fade = (buff8 == 0) ? false : true;
	file->read(reinterpret_cast<char*>(&buff32), sizeof(buff32));
	event->end = this->_time->millisecondsToTime(buff32);

	this->_events.push_back(event);
}

void Script::_loadSetSELECT(std::fstream* file)
{
	uint32_t buff32; uint16_t buff16; char buffS[255];
	memset(buffS, 0x00, 255);

	Event* event = new Event;
	event->type = EventType::SetSELECT;
	file->read(reinterpret_cast<char*>(&buff32), sizeof(buff32));
	event->start = this->_time->millisecondsToTime(buff32);
	file->read(reinterpret_cast<char*>(&buff16), sizeof(buff16));
	file->read(&buffS[0], buff16);
	event->data = std::string(buffS);
	file->read(reinterpret_cast<char*>(&buff16), sizeof(buff16));
	if (buff16 != 0x0000) {
		memset(buffS, 0x00, 255);
		file->read(&buffS[0], buff16);
		event->data = event->data + "\t" + std::string(buffS);
	}
	file->read(reinterpret_cast<char*>(&buff32), sizeof(buff32));
	event->end = this->_time->millisecondsToTime(buff32);

	this->_events.push_back(event);
}

void Script::_loadEndBGM(std::fstream* file)
{
	uint32_t buff32; uint16_t buff16; char buffS[255];
	memset(buffS, 0x00, 255);

	Event* event = new Event;
	event->type = EventType::EndBGM;
	file->read(reinterpret_cast<char*>(&buff32), sizeof(buff32));
	event->start = this->_time->millisecondsToTime(buff32);
	file->read(reinterpret_cast<char*>(&buff16), sizeof(buff16));
	file->read(&buffS[0], buff16);
	event->data = std::string(buffS);
	file->read(reinterpret_cast<char*>(&buff32), sizeof(buff32));
	event->end = this->_time->millisecondsToTime(buff32);

	this->_events.push_back(event);
}

void Script::_loadEndRoll(std::fstream* file)
{
	uint32_t buff32; uint16_t buff16; char buffS[255];
	memset(buffS, 0x00, 255);

	Event* event = new Event;
	event->type = EventType::EndRoll;
	file->read(reinterpret_cast<char*>(&buff32), sizeof(buff32));
	event->start = this->_time->millisecondsToTime(buff32);
	file->read(reinterpret_cast<char*>(&buff16), sizeof(buff16));
	file->read(&buffS[0], buff16);
	event->data = std::string(buffS);
	file->read(reinterpret_cast<char*>(&buff32), sizeof(buff32));
	event->end = this->_time->millisecondsToTime(buff32);

	this->_events.push_back(event);
}

void Script::_loadMoveSom(std::fstream* file)
{
	uint32_t buff32; uint8_t buff8;

	Event* event = new Event;
	event->type = EventType::MoveSom;
	file->read(reinterpret_cast<char*>(&buff32), sizeof(buff32));
	event->start = this->_time->millisecondsToTime(buff32);
	file->read(reinterpret_cast<char*>(&buff8), sizeof(buff8));
	event->layer = buff8;
	file->read(reinterpret_cast<char*>(&buff32), sizeof(buff32));
	event->end = this->_time->millisecondsToTime(buff32);

	this->_events.push_back(event);
}

