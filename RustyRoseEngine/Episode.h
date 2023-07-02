#pragma once
#include <vector>
#include "Script.h"
#include "BackGround.h"
#include "BackGroundMusic.h"
#include "SoundEffect.h"
#include "Voice.h"
class Episode
{
public:
	Episode(std::string path);
	/// <summary>
	/// Make simple method that make 6 lists
	/// Episode1, ..., Episode 5, System
	/// </summary>
	bool load();
	void free();

private:
	std::string _path;
	std::vector<Script*> _scripts;
	std::vector<BackGround*> _backGrounds;
	std::vector<BackGroundMusic*> _backGroundMusics;
	std::vector<SoundEffect*> _soundEffecfs;
	std::vector<Voice*> _voices;
};

