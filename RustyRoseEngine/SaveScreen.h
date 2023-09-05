#pragma once
#include "System.h"
#include "Control.h"

class SaveScreen
{
public:
	SaveScreen(SDL_Renderer* renderer, System* system, Control* control, std::string debugString, float w, float h);
	void show();

private:
	SDL_Renderer* _renderer;
	System* _system;
	Control* _control;
	
	std::string _debugString;
	float _w, _h;

	SDL_Texture* _windowScreenTexture;

};

