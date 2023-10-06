#pragma once
#include "Sound.h"

class BackGroundMusic : public Sound
{
public:
	BackGroundMusic(std::string path, SoLoud::Soloud* soloud);
	void load();
	void play();
	bool isInitDone();
	void pause();
	void resume();
	void stop();
	void setSpeed(float speed);
	void free();

	~BackGroundMusic();

private:
	bool _initDone;
	std::string _pathInit;
	std::string _pathLoop;

	SoLoud::Wav* _waveLoop;
};

