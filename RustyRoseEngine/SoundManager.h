#pragma once
#include "SharedUtils.h"
#include "Sound.h"

class SoundManager
{
public:
	SoundManager();
	
	void add(std::string path);
	void add(Sound* sound);

	Sound* get(std::string path);
	void remove(std::string path);
	
	void loadAll();
	void freeAll();

	void pauseAll();
	void resumeAll();

	void setSpeed(double speed);

	void clear();

	~SoundManager();

private:
	SoLoud::Soloud* _soloud;
	double _speed;

	std::vector<Sound*> _sounds;
};

