#include "SoundEffect.h"

SoundEffect::SoundEffect(std::string path)
{
	this->_path = path;
	this->_channel = 1;
}

void SoundEffect::setChannel(int channel)
{
	this->_channel = channel;
}

int SoundEffect::getChannel()
{
	return this->_channel;
}

void SoundEffect::load()
{
	this->_soundEffect = Mix_LoadWAV(this->_path.c_str());
	if (this->_soundEffect == NULL) {
		printf("unable to load sound effect: %s\n", this->_path.c_str());
	}
}

void SoundEffect::play()
{
	if (this == nullptr) {
		printf("ERROR - Trying to play NULL in SoundEffects\n");
		return;
	}

	this->_channel = Mix_PlayChannel(this->_channel, this->_soundEffect, 0);
	if (this->_channel == -1) {
		printf("unable to play sound effect: %s\n", this->_path.c_str());
	}
}

void SoundEffect::stop()
{
	if (this == nullptr) {
		printf("ERROR - Trying to stop NULL in SoundEffects\n");
		return;
	}

	Mix_HaltChannel(this->_channel);
}

void SoundEffect::free()
{
	if (this == nullptr) {
		printf("ERROR - Trying to free NULL in SoundEffects\n");
		return;
	}

	Mix_FreeChunk(this->_soundEffect);
	this->_soundEffect = NULL;
}

std::string SoundEffect::getPath()
{
	return this->_path;
}
