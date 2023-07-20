
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
		std::string path = this->_list[i];

		std::string format = this->_getFileFormat(path);
		if (format == ".PNG") {
			SysImg* sysimg = new SysImg(path, this->_renderer);
			this->_systemImages.push_back(sysimg);
		}
		else if (format == ".OGG") {
			SoundEffect* soundeffect = new SoundEffect(path);
			this->_soundEffects.push_back(soundeffect);
		}
		else if (format == ".ttf") {
			this->_setFont(path);
		}
		else if (format == ".rrm") {
			std::string name = this->_getFileName(path);
			auto img = this->getSystemImage(name + ".PNG");
			if (img != nullptr) {
				img->addButtons(path);
			}
			else {
				printf("cant match image to rrm: %s\n", path.c_str());
			}
		}
		else {
			printf("cant recognize format: %s in %s\n", format.c_str(), path.c_str());
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

std::string System::_getFileName(std::string path)
{
	size_t pos = path.find_last_of(".");
	
	if (pos == std::string::npos) {
		return "none";
	}

	return path.substr(0, pos);
}

std::string System::_getFileFormat(std::string path)
{
	size_t pos = path.find_last_of(".");

	if (pos == std::string::npos) {
		return "none";
	}

	return path.substr(pos, path.size()-1);
}
