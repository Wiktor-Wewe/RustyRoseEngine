#pragma once
#include "SharedUtils.h"
#include "SysImg.h"

class SaveWindow : public RustyWindow
{
public:
	SaveWindow(SDL_Renderer* renderer, RRW_ScreenSize* screenSize, RRW_Font* font);

private:
	int _selectedSlot;
	SysImg* _background;

};

