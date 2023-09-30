#include "SaveScreen.h"

SaveScreen::SaveScreen(SDL_Renderer* renderer, System* system, Control* control, std::string debugString, double screenWidth, double screenHeight, double scaleOfWindow)
{
	this->_renderer = renderer;
	this->_system = system;
	this->_control = control;
	this->_debugString = debugString;
	this->_scene = new Scene(this->_renderer);

	this->_scaleOfWindow = scaleOfWindow;

	this->_screenWidth = screenWidth;
	this->_screenHeight = screenHeight;

	this->_windowWidth = this->_screenWidth * this->_scaleOfWindow;
	this->_windowHeight = this->_screenHeight * this->_scaleOfWindow;
}

void SaveScreen::load()
{
	// set font
	this->_scene->setFont(this->_system->getFont());

	// load and add to scene background
	std::string bgPath = this->_debugString + "System\\SAVELOAD\\SAVE.PNG";
	this->_bgImage = this->_system->getSystemImage(bgPath);
	this->_bgImage->load();
	this->_scene->addSysImg(this->_bgImage, 0);
	
	// get scale between real size of image and screen
	this->_scaleOfImageX = this->_screenWidth / _bgImage->getRealSizeOfImage().w;
	this->_scaleOfImageY = this->_screenHeight / _bgImage->getRealSizeOfImage().h;

	// load buttons
	std::string buttonsPath = this->_debugString + "System\\SAVELOAD\\SAVE_HOVERS.PNG";
	this->_bgButtons = this->_system->getSystemImage(buttonsPath);
	this->_bgButtons->load();
	this->_bgButtons->addButtons(this->_debugString + "System\\SAVELOAD\\SAVE_HOVERS_MAP.rrm");

	// set dest rect to buttons and bg
	this->_shiftX = (this->_screenWidth - this->_windowWidth) / 2;
	this->_shiftY = (this->_screenHeight - this->_windowHeight) / 2;

	SDL_Rect* bgRect = new SDL_Rect;
	bgRect->x = this->_shiftX;
	bgRect->y = this->_shiftY;
	bgRect->w = this->_windowWidth;
	bgRect->h = this->_windowHeight;

	SDL_Rect* buttonsRect = new SDL_Rect;
	buttonsRect->x = this->_shiftX;
	buttonsRect->y = this->_shiftY;
	buttonsRect->w = this->_windowWidth;
	buttonsRect->h = this->_windowHeight;

	this->_bgImage->setDestRect(bgRect);
	this->_bgButtons->setDestRect(buttonsRect);
}

void SaveScreen::setPosition(int x, int y)
{
	this->_shiftX = x;
	this->_shiftY = y;
	
	SDL_Rect* bgRect = this->_bgImage->getDestRect();
	SDL_Rect* btRect = this->_bgButtons->getDestRect();

	bgRect->x = x;
	btRect->x = x;

	bgRect->y = y;
	btRect->y = y;
}

void SaveScreen::setPositionCenter()
{
	this->_shiftX = (this->_screenWidth - this->_windowWidth) / 2;
	this->_shiftY = (this->_screenHeight - this->_windowHeight) / 2;

	SDL_Rect* bgRect = this->_bgImage->getDestRect();
	SDL_Rect* btRect = this->_bgButtons->getDestRect();

	bgRect->x = this->_shiftX;
	btRect->x = this->_shiftX;

	bgRect->y = this->_shiftY;
	btRect->y = this->_shiftY;
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

	this->_scene->addSysImg(this->_bgButtons, 1);
	int i = 1;
	while (!quit)
	{
		frameStartTime = SDL_GetTicks();
		while (SDL_PollEvent(&sdl_event)) {
			if (sdl_event.type == SDL_QUIT) {
				quit = true;
			}
		}

		if (!quit) {
			this->_bgButtons->trimTexture(i);
			SDL_Rect pos = this->_bgButtons->getButtonRect(i);

			pos.x *= this->_scaleOfImageX;
			pos.y *= this->_scaleOfImageY;

			pos.x *= this->_scaleOfWindow;
			pos.y *= this->_scaleOfWindow;

			pos.x += this->_shiftX;
			pos.y += this->_shiftY;

			this->_scene->addFloatingText("TEST", pos.x, pos.y);
			this->_scene->draw();
			this->_scene->clear(Scene::Clear::floatingTextes);
			SDL_Delay(500);
			i++;
			if (i > 22) i = 1;
		}

		// fps stuff
		frameEndTime = SDL_GetTicks();
		frameTime = frameEndTime - frameStartTime;

		if (frameTime < 1000 / TARGET_FPS) {
			SDL_Delay(1000 / TARGET_FPS - frameTime);
		}

	}
}

void SaveScreen::free()
{
	this->_scene->clear(Scene::Clear::allExceptTextAndIndex);
	this->_bgImage->free();
	this->_bgButtons->free();
}

void SaveScreen::_saveGame(int id)
{
	std::string path = this->_debugString + "..\\Save\SaveFile" + std::to_string(id) + ".rrsave";
	std::fstream file(path.c_str(), std::ios::in | std::ios::binary);

	if (file.good()) {
		
	}

}
