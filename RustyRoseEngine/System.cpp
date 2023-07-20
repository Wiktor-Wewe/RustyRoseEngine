
#include "System.h"

System::System(SDL_Renderer* renderer)
{
	this->_renderer = renderer;
}

void System::setSystem(std::string path)
{
	std::fstream file;
	file.open(path.c_str(), std::ios::in);

	if (!file.good()) {
		printf("unable to open system list file: %s\n", path.c_str());
		return;
	}

	std::string line;
	while (!file.eof()) {
		std::getline(file, line);
		if (line.size() > 1) {
			this->_list.push_back(line);
		}
	}

}

void System::loadSystem()
{
	for (int i = 0; i < this->_list.size(); i++) {
		int size = this->_list[i].size();
		std::string buff = this->_list[i];
		if (buff[size - 4] == '.') {
			if (buff[size - 1] == 'G' && buff[size - 2] == 'N' && buff[size - 3] == 'P') {
				SysImg* sysimg = new SysImg(buff, this->_renderer);
				this->_systemImages.push_back(sysimg);
			}
			if (buff[size - 1] == 'G' && buff[size - 2] == 'G' && buff[size - 3] == 'O') {
				SoundEffect* soundeffect = new SoundEffect(buff);
				this->_soundEffects.push_back(soundeffect);
			}
			if (buff[size - 1] == 'f' && buff[size - 2] == 't' && buff[size - 3] == 't') {
				this->_setFont(buff);
			}
			// add if buff > 5 ???
			// add handle cmap/bin
		}
	}
}

void System::freeSystem()
{
	// delete(font)???
	for (int i = 0; i < this->_systemImages.size(); i++) {
		this->_systemImages[i]->free();
		//delete(this->_systemImages[i]);
	}
	this->_systemImages.clear();

	for (int i = 0; i < this->_soundEffects.size(); i++) {
		this->_soundEffects[i]->free();
		//delete(this->_soundEffects[i]);
	}
	this->_systemImages.clear();
}

SoundEffect* System::getSoundEffect(std::string path)
{
	for (int i = 0; i < this->_soundEffects.size(); i++) {
		if (this->_soundEffects[i]->getPath() == path) {
			return this->_soundEffects[i];
		}
	}
	return nullptr;
}

SysImg* System::getSystemImage(std::string path)
{
	for (int i = 0; i < this->_systemImages.size(); i++) {
		if (this->_systemImages[i]->getPath() == path) {
			return this->_systemImages[i];
		}
	}
	return nullptr;
}

TTF_Font* System::getFont()
{
	if (this->_font == NULL) {
		printf("Waring: font is NULL\n");
	}
	return this->_font;
}

void System::_setFont(std::string path)
{
	this->_font = TTF_OpenFont(path.c_str(), 32); // <-font size should depend of resolution of window
	if (this->_font == NULL) {
		printf("unable to load font: %s\nSDL said: %s\n", path.c_str(), TTF_GetError());
	}
}
