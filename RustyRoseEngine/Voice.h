#pragma once
#include <SDL_mixer.h>
#include <stdio.h>
#include <string>
class Voice
{
public:
	Voice(std::string path);
	// Need to convert voice from ogg to wav
	void play();
	void stop();
	void free();

private:
	std::string _path;
	Mix_Chunk* _soundEffect;
	void _load();
	int _channel;
};

