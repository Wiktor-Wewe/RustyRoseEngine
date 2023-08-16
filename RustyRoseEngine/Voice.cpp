#include "Voice.h"

Voice::Voice(std::string path)
{
	this->_path = path;
	this->_channel = 6;
}

void Voice::setChannel(int channel)
{
	this->_channel = channel;
}

int Voice::getChannel()
{
	return this->_channel;
}

void Voice::load()
{
	this->_voice = Mix_LoadWAV(this->_path.c_str());
	if (this->_voice == NULL) {
		printf("unable to load voice: %s\n", this->_path.c_str());
	}
}

void Voice::play()
{
	if (this == nullptr) {
		printf("ERROR - Trying to play NULL in Voices\n");
		return;
	}

	this->_channel = Mix_PlayChannel(this->_channel, this->_voice, 0);
	if (this->_channel == -1) {
		printf("unable to play voice: %s\n", this->_path.c_str());
	}
}

void Voice::pause()
{
	Mix_Pause(this->_channel);
}

void Voice::resume()
{
	Mix_Resume(this->_channel);
}

void Voice::stop()
{
	if (this == nullptr) {
		printf("ERROR - Trying to stop NULL in Voices\n");
		return;
	}

	Mix_HaltChannel(this->_channel);
}

void Voice::free()
{
	if (this == nullptr) {
		printf("ERROR - Trying to free NULL in Voices\n");
		return;
	}

	Mix_FreeChunk(this->_voice);
	this->_voice = NULL;
}

std::string Voice::getPath()
{
	return this->_path;
}