#include "BackGroundMusic.h"

BackGroundMusic::BackGroundMusic(std::string path)
{
	this->_path = path;
	this->_musicInt = new SoLoud::Wav();
	this->_musicLoop = new SoLoud::Wav();
	this->_nameInt = this->_endUppercase(path) + "_INT.OGG";
	this->_nameLoop = this->_endUppercase(path) + "_LOOP.OGG";
	this->_intDone = false;
}

void BackGroundMusic::load()
{
	auto result1 = this->_musicInt->load(this->_nameInt.c_str());
	if (result1 != SoLoud::SO_NO_ERROR) {
		printf("unable to load background muisc int: %s\n", this->_path.c_str());
		printf("its possible that this file just not exists\n");
	}

	auto result2 = this->_musicLoop->load(this->_nameLoop.c_str());
	if (result2 != SoLoud::SO_NO_ERROR) {
		printf("unable to load background muisc loop: %s\n", this->_path.c_str());
	}
}

void BackGroundMusic::playInt(SoLoud::Soloud* soloud)
{
	if (this == nullptr) {
		printf("ERROR - Trying to play NULL in BGM init\n");
		return;
	}

	this->_handleInt = soloud->play(*this->_musicInt);
	// unable to play background music Init: %s\n
	// its possible that this file just not exists\n
}

void BackGroundMusic::playLoop(SoLoud::Soloud* soloud)
{
	if (this == nullptr) {
		printf("ERROR - Trying to play NULL in BGM loop\n");
		return;
	}

	this->_handleInt = soloud->play(*this->_musicLoop);
	// unable to play background music Loop: %s\n
}

bool BackGroundMusic::isReadyForLoop(SoLoud::Soloud* soloud)
{
	if (soloud->isValidVoiceHandle(this->_handleInt)) {
		return false;
		this->_intDone = true;
	}
	return true;
}

void BackGroundMusic::pause(SoLoud::Soloud* soloud)
{
	if (this->_intDone) {
		soloud->setPause(this->_handleLoop, true);
	}
	else {
		soloud->setPause(this->_handleInt, true);
	}
}

void BackGroundMusic::resume(SoLoud::Soloud* soloud)
{
	if (this->_intDone) {
		soloud->setPause(this->_handleLoop, false);
	}
	else {
		soloud->setPause(this->_handleInt, false);
	}
}

void BackGroundMusic::stop(SoLoud::Soloud* soloud)
{
	soloud->stop(this->_handleInt);
	soloud->stop(this->_handleLoop);
	this->_intDone = false;
}

void BackGroundMusic::setSpeed(SoLoud::Soloud* soloud, float speed)
{
	soloud->setRelativePlaySpeed(this->_handleInt, speed);
	soloud->setRelativePlaySpeed(this->_handleLoop, speed);
}

void BackGroundMusic::free()
{
	delete this->_musicInt;
	this->_musicInt = NULL;
	delete this->_musicLoop;
	this->_musicLoop = NULL;
	this->_intDone = false;
}

std::string BackGroundMusic::getPath()
{
	return this->_path;
}

std::string BackGroundMusic::_endUppercase(std::string text)
{
	for (int i = text.size() - 1; i >= 0; i--) {
		if (text[i] == '/') {
			break;
		}

		if (text[i] > 96 && text[i] < 123) {
			text[i] = text[i] - 32;
		}
	}
	return text;
}
