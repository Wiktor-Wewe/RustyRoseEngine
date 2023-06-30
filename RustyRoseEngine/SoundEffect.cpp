#include "SoundEffect.h"

SoundEffect::SoundEffect(std::string path)
{
	this->_path = path;
	this->_load();
}

void SoundEffect::play()
{
	this->_channel = 2;
	this->_channel = Mix_PlayChannel(this->_channel, this->_soundEffect, 0);
	if (this->_channel == -1) {
		printf("unable to play sound effect: %s\n", this->_path.c_str());
	}
}

void SoundEffect::stop()
{
	Mix_HaltChannel(this->_channel);
}

void SoundEffect::free()
{
	Mix_FreeChunk(this->_soundEffect);
}

void SoundEffect::_load()
{
	this->_soundEffect = Mix_LoadWAV(this->_path.c_str());
	if (this->_soundEffect == NULL) {
		printf("unable to load sound effect: %s\n", this->_path.c_str());
	}
}
