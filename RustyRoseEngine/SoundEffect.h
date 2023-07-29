#pragma once
#include <SDL_mixer.h>
#include <stdio.h>
#include <string>
class SoundEffect
{
public:
	SoundEffect(std::string path);
	
	void setChannel(int channel);
	int getChannel();

	void play();
	void stop();
	void free();
	std::string getPath();

private:
	std::string _path;
	Mix_Chunk* _soundEffect;
	void _load();
	int _channel;
};

