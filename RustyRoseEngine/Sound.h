#pragma once
#include "SharedUtils.h"

class Sound
{
public:
	Sound(std::string path, SoLoud::Soloud* soloud);
	virtual void load();
	virtual void play();
	virtual void pause();
	virtual void resume();
	virtual void stop();
	virtual void setSpeed(float speed);
	virtual void free();

	std::string getPath();

	virtual ~Sound();

protected:
	std::string _path;
	SoLoud::Soloud* _soloud;
	SoLoud::Wav* _wave;
	SoLoud::handle _handle;

};

