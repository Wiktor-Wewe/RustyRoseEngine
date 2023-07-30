#pragma once
#include "GameContext.h"
#include "Scene.h"
#include "VDecoder.h"
#include "Timer.h"

class Game
{
public:
	struct Init {
		std::string debugString;
		std::string startScript;
		std::string linkToSys;
		int windowWidth;
		int windowHeight;
		// and more :)
	};

	Game();
	void play();
	SDL_Renderer* getRenderer();
	SDL_Window* getWindow();
	bool isGameGood();

private:
	Init _init;
	bool _gameStatus;

	std::vector<int> _freeChannelsSoundEffect;
	std::vector<int> _freeChannelsVoice;

	SDL_Renderer* _renderer;
	SDL_Window* _window;

	GameContext* _gameContext;
	Scene* _scene;
	VDecoder* _vDecoder;
	Timer* _timer;

	bool _loadInit();
	void _setDefaultInit();

	int _getFirstFreeChannelSoundEffect();
	int _getFirstFreeChannelVoice();

	//bool _containEvent(Script::Event* event, std::vector<Script::Event*>& list);
	void _removeFrom(Script::Event* event, std::vector<Script::Event*>& list);
	void _findAndHandle(Script::Event* event, int operation); // <- find action -> [operation = 0] = init, [operation = 1] = end 
	
	void _SkipFRAME_Init(Script::Event* event);
	void _SkipFRAME_End(Script::Event* event);
	
	void _PlayBgm_Init(Script::Event* event);
	void _PlayBgm_End(Script::Event* event);
	void _playLoopWhenReadyBGM(Script::Event* event);

	void _CreateBG_Init(Script::Event* event);
	void _CreateBG_End(Script::Event* event);
	
	void _PrintText_Init(Script::Event* event);
	void _PrintText_End(Script::Event* event);

	void _PlayVoice_Init(Script::Event* event);
	void _PlayVoice_End(Script::Event* event);

	void _PlaySe_Init(Script::Event* event);
	void _PlaySe_End(Script::Event* event);

	void _Next_Init(Script::Event* event);
	void _Next_End(Script::Event* event);

	void _PlayMovie_Init(Script::Event* event);
	void _PlayMovie_End(Script::Event* event);

	void _BlackFade_Init(Script::Event* event);
	void _BlackFade_End(Script::Event* event);

	void _WhiteFade_Init(Script::Event* event);
	void _WhiteFade_End(Script::Event* event);

	void _SetSELECT_Init(Script::Event* event);
	void _SetSELECT_End(Script::Event* event);

	void _EndBGM_Init(Script::Event* event);
	void _EndBGM_End(Script::Event* event);

	void _EndRoll_Init(Script::Event* event);
	void _EndRoll_End(Script::Event* event);

	void _MoveSom_Init(Script::Event* event);
	void _MoveSom_End(Script::Event* event);

	std::vector<std::string> _split(std::string text, char separator);
};

