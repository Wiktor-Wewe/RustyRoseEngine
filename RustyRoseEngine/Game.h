#pragma once
#include "GameContext.h"
#include "Scene.h"
#include "VDecoder.h"
#include "Timer.h"

class Game
{
public:
	Game();
	void play(std::string scriptPath);
	SDL_Renderer* getRenderer();
	SDL_Window* getWindow();
	bool isGameGood();

private:
	bool _initStatus;

	SDL_Renderer* _renderer;
	SDL_Window* _window;

	GameContext* _gameContext;
	Scene* _scene;
	VDecoder* _vDecoder;
	Timer* _timer;
};

