#pragma once
#include "SharedUtils.h"
#include <RustyRenderWindow.h>
#include "IniFile.h"
#include "Jumps.h"
#include "SoundManager.h"
#include "VDecoder.h"
#include "Timer.h"
#include <RustyControl.h>
#include "Script.h"
#include "BackGround.h"

class Game
{
public:
	Game();

	~Game();

private:
	bool _status;
	RustyRenderWindow* _renderWindow;
	IniFile* _iniFile;
	Jumps* _jumps;
	SoundManager* _soundManager;
	VDecoder* _vDecoder;
	Timer* _timer;
	RustyControl* _control;

	std::vector<Script*> _historyScript;
	Script* _currentScript;

	std::vector<BackGround*> _backGrounds;
};

