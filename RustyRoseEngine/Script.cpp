#include "Script.h"

Script::Script(std::string path)
{
	this->_path = path;
	
	std::fstream file;
	file.open(this->_path.c_str(), std::ios::in);

	if (!file.good()) {
		printf("unable to load script file: %s\n", this->_path.c_str());
		return;
	}

	std::string line;

	while (!file.eof()) {
		std::getline(file, line);
		if (line.size() > 1) {
			this->events.push_back(this->_lineToEvent(line));
		}
	}
}

Script::Event* Script::_lineToEvent(std::string line)
{
	Event* event = new Event;

	size_t pos = 0;
	std::string item;
	std::string delimiters = "\t=";
	std::vector<std::string> output;

	while ((pos = line.find_first_of(delimiters)) != std::string::npos) {
		item = line.substr(0, pos);
		output.push_back(item);
		line.erase(0, pos + 1);
	}
	output.push_back(line);

	event->action = this->_codeAction(output[0]);

	if (event->action == 1) { // [SkipFRAME]
		event->end = this->_stringToTime(output[1]);
	}
	else if (event->action == 2) { // [PlayBgm]
		event->start = this->_stringToTime(output[1]);
		event->data = output[2];
		event->end = this->_stringToTime(output[3]);
	}
	else if (event->action == 3) { // [CreateBG]
		event->start = this->_stringToTime(output[1]);
		event->bgshit = output[2];
		event->data = output[3];
		event->end = this->_stringToTime(output[4]);
	}
	else if (event->action == 4) {

	}
	else if (event->action == 5) {

	}
	else if (event->action == 6) {

	}
	else if (event->action == 7) {

	}
	else if (event->action == 8) {

	}
	else if (event->action == 9) {

	}
	else if (event->action == 10) {

	}
	else if (event->action == 11) {

	}
	else if (event->action == 12) {

	}
	else if (event->action == 13) {

	}

	return event;
}

int Script::_codeAction(std::string action)
{
	if (action == "[SkipFRAME]")
		return 1;
	else if (action == "[PlayBgm]")
		return 2;
	else if (action == "[CreateBG]")
		return 3;
	else if (action == "[PrintText]")
		return 4;
	else if (action == "[PlayVoice]")
		return 5;
	else if (action == "[PlaySe]")
		return 6;
	else if (action == "[Next]")
		return 7;
	else if (action == "[PlayMovie]")
		return 8;
	else if (action == "[BlackFade]")
		return 9;
	else if (action == "[WhiteFade]")
		return 10;
	else if (action == "[SetSELECT]")
		return 11;
	else if (action == "[EndBGM]")
		return 12;
	else if (action == "[EndRoll]")
		return 13;
	else
		printf("unable to code action: %s\n", action.c_str());
	return 0;
}

Script::Time* Script::_stringToTime(std::string time)
{
	Time* t = new Time;

	return t;
}
