#include "SaveScreen.h"

SaveScreen::SaveScreen(SDL_Renderer* renderer, System* system, Control* control, std::string debugString, float w, float h)
{
	this->_renderer = renderer;
	this->_system = system;
	this->_control = control;
	this->_debugString = debugString;
	this->_w = w;
	this->_h = h;

	float x = this->_w * 0.7;
	float y = this->_h * 0.7;
	this->_windowScreenTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, x, y);
}

void SaveScreen::show()
{
	bool quit = false;
	SDL_Event sdl_event;

	// fps
	const int TARGET_FPS = 60;
	Uint32 frameStartTime;
	Uint32 frameEndTime;
	Uint32 frameTime;

	std::string path = this->_debugString + "System\\SAVELOAD\\SAVE.PNG";
	SysImg* sysimg = this->_system->getSystemImage(path);
	sysimg->load();

	float textureWidth = this->_w * 0.7;
	float textureHeight = this->_h * 0.7;
	float x = (this->_w - textureWidth) / 2;
	float y = (this->_h - textureHeight) / 2;

	SDL_Rect rect{ x, y, textureWidth, textureHeight };

	while (!quit)
	{
		frameStartTime = SDL_GetTicks();
		while (SDL_PollEvent(&sdl_event)) {
			if (sdl_event.type == SDL_QUIT) {
				quit = true;
			}
		}

		if (!quit) {

			SDL_SetRenderTarget(this->_renderer, this->_windowScreenTexture);
			SDL_RenderCopy(this->_renderer, sysimg->getTexture(), NULL, NULL);
			
			SDL_SetRenderTarget(this->_renderer, NULL);
			SDL_RenderCopy(this->_renderer, this->_windowScreenTexture, NULL, &rect);
			SDL_RenderPresent(this->_renderer);

		}

		// fps stuff
		frameEndTime = SDL_GetTicks();
		frameTime = frameEndTime - frameStartTime;

		if (frameTime < 1000 / TARGET_FPS) {
			SDL_Delay(1000 / TARGET_FPS - frameTime);
		}

	}
}
