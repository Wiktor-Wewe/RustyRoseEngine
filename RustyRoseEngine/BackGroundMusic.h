#pragma once
#include <SDL_mixer.h>
#include <stdio.h>
#include <string>
class BackGroundMusic
{
public:
	BackGroundMusic(std::string path);

	void play();
	void stop();
	void free();

private:
	std::string _path;
	Mix_Music* _music;
	void _load();

};

