#pragma once
#include "SharedUtils.h"

class Sound
{
public:
	Sound(std::string path, SoLoud::Soloud* soloud);
	void load();
	void play();
	void pause();
	void resume();
	void stop();
	void setSpeed(float speed);
	void free();

	std::string getPath();

	~Sound();

protected:
	std::string _path;
	SoLoud::Soloud* _soloud;
	SoLoud::Wav* _wave;
	SoLoud::handle _handle;

};

