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
	int _channel;
	bool _int;
	Mix_Chunk* _musicInt;
	Mix_Chunk* _musicLoop;
	void _load();

};

