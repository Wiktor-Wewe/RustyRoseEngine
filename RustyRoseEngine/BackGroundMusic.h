#pragma once
#include <SDL_mixer.h>
#include <stdio.h>
#include <string>
class BackGroundMusic
{
public:
	BackGroundMusic(std::string path); // <- path without _xxx/x_.ogg

	void load();
	void playInt();
	void playLoop();
	bool isReadyForLoop();
	void stop();
	void free();

	std::string getPath();

private:
	std::string _path;
	std::string _nameInt;
	std::string _nameLoop;
	int _channel;
	bool _int;
	Mix_Chunk* _musicInt;
	Mix_Chunk* _musicLoop;

	std::string _endUppercase(std::string text);
};

