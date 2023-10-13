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
	void play();
	bool getStatus();

	~Game();

private:
	// engine
	bool _status;
	IniFile* _iniFile;
	RustyRenderWindow* _renderWindow;
	Jumps* _jumps;
	SoundManager* _soundManager;
	VDecoder* _vDecoder;
	Timer* _timer;
	RustyControl* _control;

	std::vector<Script*> _historyScript;
	Script* _currentScript;

	std::vector<BackGround*> _backGrounds;

	// gameplay
	bool _pauseStatus;

	void _PlayBgm_Load(Script::Event* event);
	void _PlayBgm_Start(Script::Event* event);
	void _PlayBgm_Loop(Script::Event* event);
	void _PlayBgm_End(Script::Event* event);

	void _CreateBG_Load(Script::Event* event);
	void _CreateBG_Start(Script::Event* event);
	void _CreateBG_End(Script::Event* event);

	void _PrintText_Start(Script::Event* event);
	void _PrintText_End(Script::Event* event);

	void _PlayVoice_Load(Script::Event* event);
	void _PlayVoice_Start(Script::Event* event);
	void _PlayVoice_End(Script::Event* event);

	void _PlaySe_Load(Script::Event* event);
	void _PlaySe_Start(Script::Event* event);
	void _PlaySe_End(Script::Event* event);

	void _Next_(Script::Event* event);

	void _PlayMovie_Start(Script::Event* event);
	void _PlayMovie_Loop(Script::Event* event);
	void _PlayMovie_End(Script::Event* event);

	void _BlackFade_Start(Script::Event* event);
	void _BlackFade_End(Script::Event* event);

	void _WhiteFade_Start(Script::Event* event);
	void _WhiteFade_End(Script::Event* event);

	void _SetSELECT_Start(Script::Event* event);
	void _SetSELECT_Loop(Script::Event* event);
	void _SetSELECT_End(Script::Event* event);

	void _EndBGM_Load(Script::Event* event);
	void _EndBGM_Start(Script::Event* event);
	void _EndBGM_End(Script::Event* event);

	void _EndRoll_Start(Script::Event* event);
	void _EndRoll_Loop(Script::Event* event);
	void _EndRoll_End(Script::Event* event);

	// vibration for controler xD yeah im genius
	void _MoveSom_Start(Script::Event* event);
	void _MoveSom_End(Script::Event* event);

	void _pause();
	void _resume();
};

