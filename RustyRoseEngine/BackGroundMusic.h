#pragma once
#include <SDL_mixer.h>
#include <stdio.h>
#include <string>
class BackGroundMusic
{
public:
	BackGroundMusic(std::string path); // <- path without _xxx/x_.ogg

	void playInt();
	void playLoop();
	bool isReadyForLoop();
	void stop();
	void free();

private:
	std::string _path;
	std::string _nameInt;
	std::string _nameLoop;
	bool _int;
	Mix_Music* _musicInt;
	Mix_Music* _musicLoop;
	void _load();

};

