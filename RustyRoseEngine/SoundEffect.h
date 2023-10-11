#pragma once
#include "Sound.h"

class SoundEffect : public Sound
{
public:
	SoundEffect(std::string path, SoLoud::Soloud* soloud, double speed);

private:

};

