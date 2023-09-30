#include "SysImg.h"

SysImg::SysImg(std::string path, SDL_Renderer* renderer)
{
	this->_path = path;
	this->_renderer = renderer;
	this->_destRect = NULL;
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

	if (this->_texture != NULL) {
		SDL_DestroyTexture(this->_texture);
		this->_texture = NULL;
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

	float scalex = 1280.f / 800.f;
	float scaley = 720.f / 450.f;

	SDL_Rect srcRect = { button->x, button->y, button->w, button->h };
	SDL_Rect dstRect = { button->x * scalex, button->y * scaley, button->w * scalex, button->h * scaley };
	
	this->_texture = SDL_CreateTexture(this->_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 1280, 720);
	SDL_SetTextureBlendMode(this->_texture, SDL_BLENDMODE_BLEND);
	SDL_SetRenderTarget(this->_renderer, this->_texture);
	SDL_Texture* tempTexture = SDL_CreateTextureFromSurface(this->_renderer, this->_surface);
	SDL_RenderCopy(this->_renderer, tempTexture, &srcRect, &dstRect);
	SDL_DestroyTexture(tempTexture);
	SDL_SetRenderTarget(this->_renderer, NULL);
}

void SysImg::setDestRect(SDL_Rect* rect)
{
	this->_destRect = rect;
}

SDL_Rect* SysImg::getDestRect()
{
	return this->_destRect;
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

SDL_Rect SysImg::getButtonRect(int id)
{
	auto button = this->_getButton(id);
	if (!button) {
		return SDL_Rect{ 0, 0, 0, 0 };
	}

	SDL_Rect rect;
	rect.x = button->x;
	rect.y = button->y;
	rect.w = button->w;
	rect.h = button->h;
	
	return rect;
}

SDL_Rect SysImg::getRealSizeOfImage()
{
	return SDL_Rect {0, 0, this->_surface->w, this->_surface->h};
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

	if (this->_destRect) {
		delete this->_destRect;
		this->_destRect = NULL;
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
