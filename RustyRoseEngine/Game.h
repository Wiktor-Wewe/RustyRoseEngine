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
		end = 2
	};

	struct Init {
		std::string debugString;
		std::string startScript;
		std::string linkToSys;
		int windowWidth;
		int windowHeight;
		std::string linkToJump;
		// and more :)
	};

	struct Jump
	{
		int id;
		int routeId;
		std::string scriptName;
		int jumpToId;
	};

	Game();
	void play();
	SDL_Renderer* getRenderer();
	SDL_Window* getWindow();
	bool isGameGood();

private:
	Init _init;
	bool _gameStatus;

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
	void _removeFrom(Script::Event* event, std::vector<Script::Event*>& list);
	void _findAndHandle(Script::Event* event, Operation operation); // <- find action -> [operation = 0] = prepare, [operation = 1] = start, [operation = 2] = end + free
	
	void _SkipFRAME_(Script::Event* event);
	
	void _PlayBgm_Prepare(Script::Event* event);
	void _PlayBgm_Start(Script::Event* event);
	void _PlayBgm_End(Script::Event* event);
	void _playLoopWhenReadyBGM(Script::Event* event);

	void _CreateBG_Prepare(Script::Event* event);
	void _CreateBG_Start(Script::Event* event);
	void _CreateBG_End(Script::Event* event);
	
	void _PrintText_Start(Script::Event* event);
	void _PrintText_End(Script::Event* event);

	void _PlayVoice_Prepare(Script::Event* event);
	void _PlayVoice_Start(Script::Event* event);
	void _PlayVoice_End(Script::Event* event);

	void _PlaySe_Prepare(Script::Event* event);
	void _PlaySe_Start(Script::Event* event);
	void _PlaySe_End(Script::Event* event);

	void _Next_(Script::Event* event);

	// void _PlayMovie_Prepare?
	void _PlayMovie_Start(Script::Event* event);
	void _PlayMovie_End(Script::Event* event);

	void _BlackFade_Start(Script::Event* event);
	void _BlackFade_End(Script::Event* event);

	void _WhiteFade_Start(Script::Event* event);
	void _WhiteFade_End(Script::Event* event);

	void _SetSELECT_Start(Script::Event* event);
	void _SetSELECT_End(Script::Event* event);

	void _EndBGM_Prepare(Script::Event* event);
	void _EndBGM_Start(Script::Event* event);
	void _EndBGM_End(Script::Event* event);

	// just like play movie but its end of episode, so after video show save screen
	// void _EndRoll_Prepare?
	void _EndRoll_Start(Script::Event* event);
	void _EndRoll_End(Script::Event* event);

	// vibration for controler xD yeah im genius
	void _MoveSom_Start(Script::Event* event);
	void _MoveSom_End(Script::Event* event);

	std::vector<std::string> _split(std::string text, char separator);
	std::string _findNameOfScriptById(int scriptId);
	int _findNextScrpitId(std::string currentScriptName, int playerOption);
	void _wipeCharArr(char* arr, int size);
};

