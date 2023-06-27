#include "Voice.h"

Voice::Voice(std::string path)
{
	this->_path = path;
	this->_load();
}

void Voice::play(int channel)
{
	this->_channel = Mix_PlayChannel(this->_channel, this->_soundEffect, 0);
	if (this->_channel == -1) {
		printf("unable to play voice: %s\n", this->_path.c_str());
	}
}

void Voice::stop()
{
	Mix_HaltChannel(this->_channel);
}

void Voice::free()
{
	Mix_FreeChunk(this->_soundEffect);
}

void Voice::_load()
{
	this->_soundEffect = Mix_LoadWAV(this->_path.c_str());
	if (this->_soundEffect == NULL) {
		printf("unable to load voice: %s\n", this->_path.c_str());
	}
}
