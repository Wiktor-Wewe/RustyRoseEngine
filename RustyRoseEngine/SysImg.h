#pragma once
#include "SharedUtils.h"
#include "Image.h"

class SysImg : public Image
{
public:
	SysImg(SDL_Renderer* renderer, RRW_ScreenSize* screenSize, std::string path);

private:

};

