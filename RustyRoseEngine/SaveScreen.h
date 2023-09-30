#pragma once
#include "System.h"
#include "Control.h"
#include "Scene.h"

class SaveScreen
{
public:
	SaveScreen(SDL_Renderer* renderer, System* system, Control* control, std::string debugString, double screenWidth, double screenHeight, double scaleOfWindow);
	void load();
	void setPosition(int x, int y);
	void setPositionCenter();
	void show();
	void free();

private:
	SDL_Renderer* _renderer;
	System* _system;
	Control* _control;
	Scene* _scene;
	
	std::string _debugString;
	double _scaleOfWindow;
	double _screenWidth, _screenHeight;
	double _windowWidth, _windowHeight;
	double _shiftX, _shiftY;
	double _scaleOfImageX, _scaleOfImageY;

	SysImg* _bgImage;
	SysImg* _bgButtons;

	void _saveGame(int id);
};

