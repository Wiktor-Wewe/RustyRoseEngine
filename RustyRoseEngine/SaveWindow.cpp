#include "SaveWindow.h"

SaveWindow::SaveWindow(SDL_Renderer* renderer, RRW_ScreenSize* screenSize, RRW_Font* font) : RustyWindow(renderer, screenSize, font, screenSize->Width, screenSize->Height)
{
	this->_background = new SysImg(renderer, screenSize, "C:/Users/Wiktor/source/repos/RustyRoseEngine/x64/Debug/data/System/SAVELOAD/SAVE.PNG");
	this->_background->load();
	this->_background->loadTrimInstructions("C:/Users/Wiktor/source/repos/RustyRoseEngine/x64/Debug/data/System/SAVELOAD/SAVE_HOVERS_MAP.rrm");
	this->setCustomTexture(this->_background->getTexture());

	this->setBackgroundColor({ 0x00, 0x00, 0x00, 0x50 });
	this->hideBar();
	this->setPosition(0, 0);
	this->addText("Save Screen :)", 0, 0, nullptr);

	SDL_Rect rect;
	int id;
	for (auto& trim : this->_background->getAllTrimInstructions()) {
		if (trim.first == 0) continue;
		RRE_ScaleRectToScreenSize(screenSize, this->_background->getOriginalImageSize(), trim.second, &rect);
		id = this->addButton(std::to_string(trim.first), rect.x, rect.y, rect.w, rect.h);
		this->getButton(id)->setBackgroundColor({ 0x00, 0x00, 0x00, 0x00 });
	}
	
}
