#pragma once
#include <SDL_mixer.h>
#include <stdio.h>
#include <string>
class SoundEffect
{
public:
	SoundEffect(std::string path);
	// Need to convert sound effect from ogg to wav
	void play();
	void stop();
	void free();

private:
	std::string _path;
	Mix_Chunk* _soundEffect;
	void _load();
	int _channel;
};

