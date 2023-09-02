#pragma once
#include <soloud_wav.h>
#include <stdio.h>
#include <string>
class SoundEffect
{
public:
	SoundEffect(std::string path);
	
	void load();

	void play(SoLoud::Soloud* soloud);
	void pause(SoLoud::Soloud* soloud);
	void resume(SoLoud::Soloud* soloud);
	void stop(SoLoud::Soloud* soloud);
	void setSpeed(SoLoud::Soloud* soloud, float speed);
	void free();
	std::string getPath();

private:
	std::string _path;
	SoLoud::Wav* _soundEffect;
	SoLoud::handle _handle;
};

