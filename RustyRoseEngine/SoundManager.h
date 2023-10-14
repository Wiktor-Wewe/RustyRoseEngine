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
	int getCurrentSpeed();
	
	void loadAll();
	void freeAll();

	void pauseAll();
	void resumeAll();

	void setSpeed(int level);
	void clear();

	~SoundManager();

private:
	SoLoud::Soloud* _soloud;
	int _speedLevels[5] = {1, 2, 4, 16, 32};
	int _currentSpeedLevelIndex = 0;

	std::vector<Sound*> _sounds;
};

