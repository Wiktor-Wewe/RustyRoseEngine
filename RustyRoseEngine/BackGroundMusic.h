#pragma once
#include <soloud_wav.h>
#include <stdio.h>
#include <string>
class BackGroundMusic
{
public:
	BackGroundMusic(std::string path); // <- path without _xxx/x_.ogg

	void load();
	void playInt(SoLoud::Soloud* soloud);
	void playLoop(SoLoud::Soloud* soloud);
	bool isReadyForLoop(SoLoud::Soloud* soloud);
	void pause(SoLoud::Soloud* soloud);
	void resume(SoLoud::Soloud* soloud);
	void stop(SoLoud::Soloud* soloud);
	void free();

	std::string getPath();

private:
	bool _intDone;
	std::string _path;
	std::string _nameInt;
	std::string _nameLoop;
	SoLoud::handle _handleInt;
	SoLoud::handle _handleLoop;
	SoLoud::Wav* _musicInt;
	SoLoud::Wav* _musicLoop;

	std::string _endUppercase(std::string text);
};

