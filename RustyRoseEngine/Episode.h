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
	Episode();
	/// <summary>
	/// Trzeba zrobi� list� wszytkich plik�w posegregowanych po 
	/// epizodach - najlepiej �eby backgoud mia�y .png ale ju�
	/// backgroudmusic - mia�o tylko �cie�k� do folderu
	/// </summary>
	void load();
	void free();

private:
	std::vector<Script*> _scripts;
	std::vector<BackGround*> _backGrounds;
	std::vector<BackGroundMusic*> _backGroundMusics;
	std::vector<SoundEffect*> _soundEffecfs;
	std::vector<Voice*> _voices;
};

