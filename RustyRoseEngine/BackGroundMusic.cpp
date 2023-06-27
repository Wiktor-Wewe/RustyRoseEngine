#include "BackGroundMusic.h"

BackGroundMusic::BackGroundMusic(std::string path)
{
	this->_path = path;
	this->_nameInt = path + "_INT.OGG";
	this->_nameLoop = path + "_LOOP.OGG";
	this->_load();
}

void BackGroundMusic::playInt()
{
	if (Mix_PlayMusic(this->_musicInt, 0) == -1) {
		printf("unable to play background music Init: %s\n", this->_nameInt.c_str());
		printf("its possible that this file just not exists\n");
	}
}

void BackGroundMusic::playLoop()
{
	if (Mix_PlayMusic(this->_musicLoop, -1) == -1) {
		printf("unable to play background music Loop: %s\n", this->_nameLoop.c_str());
	}
}

bool BackGroundMusic::isReadyForLoop()
{
	if (Mix_PlayingMusic() == 0) {
		return true;
	}
	return false;
}

void BackGroundMusic::stop()
{
	Mix_HaltMusic();
}

void BackGroundMusic::free()
{
	Mix_FreeMusic(this->_musicInt);
	Mix_FreeMusic(this->_musicLoop);
}

void BackGroundMusic::_load()
{
	this->_musicInt = Mix_LoadMUS(this->_nameInt.c_str());
	if (this->_musicInt == NULL) {
		printf("unable to load background muisc int: %s\n", this->_nameInt.c_str());
		printf("its possible that this file just not exists\n");
	}

	this->_musicLoop = Mix_LoadMUS(this->_nameLoop.c_str());
	if (this->_musicLoop == NULL) {
		printf("unable to load background muisc loop: %s\n", this->_nameLoop.c_str());
	}
}
