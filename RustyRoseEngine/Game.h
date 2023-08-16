#pragma once
#include "GameContext.h"
#include "Scene.h"
#include "VDecoder.h"
#include "Timer.h"
#include "Control.h"

class Game
{
public:
	enum Operation {
		prepare = 0,
		start = 1,
		end = 2,
		loop = 3,
		pause = 4,
		resume = 5
	};

	enum Command {
		previousScript = -10
	};

	struct Init {
		std::string debugString;
		std::string startScript;
		std::string linkToSys;
		int windowWidth = 1280;
		int windowHeight = 720;
		std::string linkToJump;
		// and more :)
	};

	struct Jump
	{
		int id = 0;
		int routeId = 0;
		std::string scriptName;
		int jumpToId = 0;
	};

	Game();
	void play();
	SDL_Renderer* getRenderer();
	SDL_Window* getWindow();
	bool isGameGood();

private:
	Init _init;
	bool _gameStatus;
	std::vector<Script*> _scriptHistory;

	SDL_mutex* _eventMutex;

	std::vector<int> _freeChannelsSoundEffect;
	std::vector<int> _freeChannelsVoice;

	SDL_Renderer* _renderer;
	SDL_Window* _window;

	GameContext* _gameContext;
	Scene* _scene;
	VDecoder* _vDecoder;
	Timer* _timer;
	Control _control;

	std::vector<Jump*> _jumps;

	int _playScripts();

	bool _loadJumps();
	bool _loadInit();
	void _setDefaultInit();

	int _getFirstFreeChannelSoundEffect();
	int _getFirstFreeChannelVoice();

	//bool _containEvent(Script::Event* event, std::vector<Script::Event*>& list);
	void _removeFrom(Script* element, std::vector<Script*>& list);
	
	void _findAndHandle(Script::Event* event, Operation operation);
	void _handleControl(bool& quit, bool& isOkayToSkip, Script::Event* setSELECT, Script::Event* currEvent, int& extraCommand, bool& pause, std::vector<Script::Event*>& inprogres);

	void _SkipFRAME_(Script::Event* event);
	
	void _PlayBgm_Prepare(Script::Event* event);
	void _PlayBgm_Start(Script::Event* event);
	void _PlayBgm_End(Script::Event* event);
	void _PlayBgm_Loop(Script::Event* event);
	void _PlayBgm_Pause(Script::Event* event);
	void _PlayBgm_Resume(Script::Event* event);

	void _CreateBG_Prepare(Script::Event* event);
	void _CreateBG_Start(Script::Event* event);
	void _CreateBG_End(Script::Event* event);
	void _CreateBG_Pause(Script::Event* event);
	void _CreateBG_Resume(Script::Event* event);
	
	void _PrintText_Start(Script::Event* event);
	void _PrintText_End(Script::Event* event);

	void _PlayVoice_Prepare(Script::Event* event);
	void _PlayVoice_Start(Script::Event* event);
	void _PlayVoice_End(Script::Event* event);
	void _PlayVoice_Pause(Script::Event* event);
	void _PlayVoice_Resume(Script::Event* event);

	void _PlaySe_Prepare(Script::Event* event);
	void _PlaySe_Start(Script::Event* event);
	void _PlaySe_End(Script::Event* event);
	void _PlaySe_Pause(Script::Event* event);
	void _PlaySe_Resume(Script::Event* event);

	void _Next_(Script::Event* event);

	// void _PlayMovie_Prepare?
	void _PlayMovie_Start(Script::Event* event);
	void _PlayMovie_End(Script::Event* event);
	void _PlayMovie_Loop(Script::Event* event);
	void _PlayMovie_Pause(Script::Event* event);
	void _PlayMovie_Resume(Script::Event* event);

	void _BlackFade_Start(Script::Event* event);
	void _BlackFade_End(Script::Event* event);

	void _WhiteFade_Start(Script::Event* event);
	void _WhiteFade_End(Script::Event* event);

	void _SetSELECT_Prepare(Script::Event* event);
	void _SetSELECT_Start(Script::Event* event);
	void _SetSELECT_End(Script::Event* event);

	void _EndBGM_Prepare(Script::Event* event);
	void _EndBGM_Start(Script::Event* event);
	void _EndBGM_End(Script::Event* event);
	void _EndBGM_Pause(Script::Event* event);
	void _EndBGM_Resume(Script::Event* event);

	// just like play movie but its end of episode, so after video show save screen
	// void _EndRoll_Prepare?
	void _EndRoll_Start(Script::Event* event);
	void _EndRoll_End(Script::Event* event);
	void _EndRoll_Loop(Script::Event* event);
	void _EndRoll_Pause(Script::Event* event);
	void _EndRoll_Resume(Script::Event* event);

	// vibration for controler xD yeah im genius
	void _MoveSom_Start(Script::Event* event);
	void _MoveSom_End(Script::Event* event);
	void _MoveSom_Pause(Script::Event* event);
	void _MoveSom_Resume(Script::Event* event);

	std::vector<std::string> _split(std::string text, char separator);
	std::string _findNameOfScriptById(int scriptId);
	int _findNextScrpitId(std::string currentScriptName, int playerOption);
	void _wipeCharArr(char* arr, int size);
};