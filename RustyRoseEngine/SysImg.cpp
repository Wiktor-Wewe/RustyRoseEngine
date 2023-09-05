#include "SysImg.h"

SysImg::SysImg(std::string path, SDL_Renderer* renderer)
{
	this->_path = path;
	this->_renderer = renderer;
}

bool SysImg::load()
{
	this->_surface = IMG_Load(this->_path.c_str());
	if (this->_surface == NULL) {
		printf("unable to load SysImg surface: %s\n", this->_path.c_str());
		return false;
	}
	this->trimTexture(0);
	return true;
}

SDL_Texture* SysImg::getTexture()
{
	if (this == NULL) {
		printf("ERROR - TRY TO GET SYSTEM IMG TEXTURE FROM NULL\n");
		return NULL;
	}
	return this->_texture;
}

void SysImg::trimTexture(int id)
{
	if (this->_surface == NULL) {
		printf("unable to trim texture: %s\nSurface is NULL\n", this->_path.c_str());
		return;
	}

	if (id == 0) {
		this->_texture = SDL_CreateTextureFromSurface(this->_renderer, this->_surface);
		if (this->_texture == NULL) {
			printf("unable to make texture from surface: %s\n", this->_path.c_str());
		}
		return;
	}

	Button* button = this->_getButton(id);
	if (button == nullptr) {
		printf("unable to trim texture: %s\nbutton with id: %i not found\n", this->_path.c_str(), id);
		return;
	}

	SDL_Surface* tempSurface = SDL_CreateRGBSurfaceWithFormat(0, button->w, button->h, 32, SDL_PIXELFORMAT_RGBA32);
	SDL_Rect srcRect = { button->x, button->y, button->w, button->h };
	SDL_Rect dstRect = { 0, 0, button->w, button->h };
	SDL_BlitSurface(this->_surface, &srcRect, tempSurface, &dstRect);
	this->_texture = SDL_CreateTextureFromSurface(this->_renderer, tempSurface);
	SDL_FreeSurface(tempSurface);
}

void SysImg::addButtons(std::string path)
{
	std::fstream file;
	file.open(path.c_str(), std::ios::in | std::ios::binary);
	if (!file.good()) {
		printf("unable to add button: %s\nunable to open file\n", path.c_str());
		return;
	}

	OriginalSize* originalSize = new OriginalSize;

	uint32_t buff32;
	
	char header[4];
	file.read(reinterpret_cast<char*>(&header[0]), 4);
	
	if (strncmp(header, "rrmf", 4) != 0) {
		printf("unable to add button: %s\nheader error\n", path.c_str());
		return;
	}

	file.read(reinterpret_cast<char*>(&buff32), sizeof(buff32));
	originalSize->x = buff32;
	file.read(reinterpret_cast<char*>(&buff32), sizeof(buff32));
	originalSize->y = buff32;

	this->_originalSize = originalSize;

	file.read(reinterpret_cast<char*>(&buff32), sizeof(buff32));
	int count = buff32;

	for (int i = 0; i < count; i++) {
		Button* button = new Button;
		file.read(reinterpret_cast<char*>(&buff32), sizeof(buff32));
		button->id = buff32;
		file.read(reinterpret_cast<char*>(&buff32), sizeof(buff32));
		button->x = buff32;
		file.read(reinterpret_cast<char*>(&buff32), sizeof(buff32));
		button->y = buff32;
		file.read(reinterpret_cast<char*>(&buff32), sizeof(buff32));
		button->w = buff32;
		file.read(reinterpret_cast<char*>(&buff32), sizeof(buff32));
		button->h = buff32;
		this->_buttons.push_back(button);
	}

	file.close();
}

std::string SysImg::getPath()
{
	return this->_path;
}

void SysImg::free()
{
	SDL_DestroyTexture(this->_texture);
	this->_texture = NULL;
	SDL_FreeSurface(this->_surface);
	this->_surface = NULL;

	if (this->_originalSize) {
		delete this->_originalSize;
		this->_originalSize = nullptr;
	}

	for (int i = 0; i < this->_buttons.size(); i++) {
		delete this->_buttons[i];
	}
	this->_buttons.clear();
}

SysImg::Button* SysImg::_getButton(int id)
{
	for (int i = 0; i < this->_buttons.size(); i++) {
		if (this->_buttons[i]->id == id) {
			return this->_buttons[i];
		}
	}
	return nullptr;
}
