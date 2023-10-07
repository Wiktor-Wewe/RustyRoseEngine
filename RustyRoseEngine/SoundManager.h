#pragma once
#include "SharedUtils.h"
#include "Sound.h"

class SoundManager
{
public:
	SoundManager();

	~SoundManager();

private:
	SoLoud::Soloud* _soloud;
	int _speedLevels[5] = {1, 2, 4, 16, 32};
	int _currentSpeedLevelIndex = 0;

};

