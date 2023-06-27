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
	/// Trzeba zrobiæ listê wszytkich plików posegregowanych po 
	/// epizodach - najlepiej ¿eby backgoud mia³y .png ale ju¿
	/// backgroudmusic - mia³o tylko œcie¿kê do folderu
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

