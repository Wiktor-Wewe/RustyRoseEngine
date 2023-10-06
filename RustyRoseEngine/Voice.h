#pragma once
#include "Sound.h"

class Voice : public Sound
{
public:
	Voice(std::string path, SoLoud::Soloud* soloud);

private:
};

