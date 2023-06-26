#include "BackGroundMusic.h"

BackGroundMusic::BackGroundMusic(std::string path)
{
	this->_path = path;
	this->_load();
}

void BackGroundMusic::play()
{
	if (Mix_PlayMusic(this->_music, -1) == -1) {
		printf("unable to play background music: %s\n", this->_path.c_str());
	}
}

void BackGroundMusic::stop()
{
	Mix_HaltMusic();
}

void BackGroundMusic::free()
{
	Mix_FreeMusic(this->_music);
}

void BackGroundMusic::_load()
{
	this->_music = Mix_LoadMUS(this->_path.c_str());
	if (this->_music == NULL) {
		printf("unable to load background muisc: %s\n", this->_path.c_str());
	}
}
