#include "Image.h"

Image::Image(SDL_Renderer* renderer, RRW_ScreenSize* screenSize, std::string path)
{
	this->_renderer = renderer;
	this->_screenSize = screenSize;
	this->_path = path;
	this->_texture = NULL;

	this->_textureToTrim = NULL;
	this->_originalImageSize = nullptr;
	this->_trimedTexture = NULL;
}

void Image::load()
{
	if (this == NULL) {
		printf("Error - Trying to load NULL in image\n");
		return;
	}

	SDL_Surface* surface = IMG_Load(this->_path.c_str());
	if (surface == NULL) {
		printf("unable to load image surface: %s\n", this->_path.c_str());
		return;
	}

	this->_texture = SDL_CreateTextureFromSurface(this->_renderer, surface);
	if (this->_texture == NULL) {
		printf("unable to create image texture: %s\n", this->_path.c_str());
	}

	this->_textureToTrim = this->_texture;
	SDL_FreeSurface(surface);
}

void Image::loadTrimInstructions(std::string path)
{
	std::fstream file;
	file.open(path.c_str(), std::ios::in | std::ios::binary);
	if (!file.good()) {
		printf("unable to add trim instruction: %s\nunable to open file\n", path.c_str());
		return;
	}

	this->_originalImageSize = new RRW_ScreenSize(0, 0);

	uint32_t buff32;

	char header[4];
	file.read(reinterpret_cast<char*>(&header[0]), 4);

	if (strncmp(header, "rrmf", 4) != 0) {
		printf("unable to add trim istructions: %s\nheader error\n", path.c_str());
		return;
	}

	file.read(reinterpret_cast<char*>(&buff32), sizeof(buff32));
	this->_originalImageSize->Width = buff32;
	file.read(reinterpret_cast<char*>(&buff32), sizeof(buff32));
	this->_originalImageSize->Height = buff32;

	file.read(reinterpret_cast<char*>(&buff32), sizeof(buff32));
	int count = buff32;
	int id = 0;

	for (int i = 0; i < count; i++) {
		SDL_Rect* rect = new SDL_Rect;
		file.read(reinterpret_cast<char*>(&buff32), sizeof(buff32));
		id = buff32;
		file.read(reinterpret_cast<char*>(&buff32), sizeof(buff32));
		rect->x = buff32;
		file.read(reinterpret_cast<char*>(&buff32), sizeof(buff32));
		rect->y = buff32;
		file.read(reinterpret_cast<char*>(&buff32), sizeof(buff32));
		rect->w = buff32;
		file.read(reinterpret_cast<char*>(&buff32), sizeof(buff32));
		rect->h = buff32;
		this->_trimInstructions[id] = rect;
	}

	file.close();
}

void Image::free()
{
	if (this->_texture) {
		SDL_DestroyTexture(this->_texture);
		this->_texture = NULL;
	}

	if (this->_textureToTrim) {
		SDL_DestroyTexture(this->_textureToTrim);
		this->_textureToTrim = NULL;
	}

	if (this->_trimedTexture) {
		SDL_DestroyTexture(this->_trimedTexture);
		this->_trimedTexture = NULL;
	}
}

void Image::setTextureToTrim(std::string path)
{
	if (this == NULL) {
		printf("Error - Trying to load trim NULL in image\n");
		return;
	}

	SDL_Surface* surface = IMG_Load(this->_path.c_str());
	if (surface == NULL) {
		printf("unable to load image surface to trim: %s\n", this->_path.c_str());
		return;
	}

	this->_textureToTrim = SDL_CreateTextureFromSurface(this->_renderer, surface);
	if (this->_textureToTrim == NULL) {
		printf("unable to create image texture for trim: %s\n", this->_path.c_str());
		this->_textureToTrim = this->_texture;
	}

	SDL_FreeSurface(surface);
}

void Image::trimTexture(int id)
{
	if (id == 0) {
		if (this->_trimedTexture) {
			SDL_DestroyTexture(this->_trimedTexture);
			this->_trimedTexture = this->_texture;
		}
	}

	SDL_Rect* rect = this->_trimInstructions[id];
	if (rect == nullptr) {
		printf("unable to trim texture: %s\instruction with id: %i not found\n", this->_path.c_str(), id);
		return;
	}

	float scalex = (float)this->_screenSize->Width / (float)this->_originalImageSize->Width;
	float scaley = (float)this->_screenSize->Height / (float)this->_originalImageSize->Height;

	SDL_Rect srcRect = { rect->x, rect->y, rect->w, rect->h };
	SDL_Rect dstRect = { rect->x * scalex, rect->y * scaley, rect->w * scalex, rect->h * scaley };

	if (this->_trimedTexture) {
		SDL_DestroyTexture(this->_trimedTexture);
		this->_trimedTexture = NULL;
	}

	this->_trimedTexture = SDL_CreateTexture(this->_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, this->_screenSize->Width, this->_screenSize->Height);
	auto oldTarget = SDL_GetRenderTarget(this->_renderer);

	SDL_SetTextureBlendMode(this->_trimedTexture, SDL_BLENDMODE_BLEND);
	SDL_SetRenderTarget(this->_renderer, this->_trimedTexture);
	SDL_RenderCopy(this->_renderer, this->_textureToTrim, &srcRect, &dstRect);

	SDL_SetRenderTarget(this->_renderer, oldTarget);
}

std::string Image::getPath()
{
	return this->_path;
}

SDL_Texture* Image::getTexture()
{
	return this->_texture;
}

SDL_Texture* Image::getTrimedTexture()
{
	return this->_trimedTexture;
}

SDL_Rect* Image::getRectOfTrim(int id)
{
	return this->_trimInstructions[id];
}

Image::~Image()
{
	if (this->_texture) {
		SDL_DestroyTexture(this->_texture);
		this->_texture = NULL;
	}

	if (this->_textureToTrim) {
		SDL_DestroyTexture(this->_textureToTrim);
		this->_textureToTrim = NULL;
	}

	if (this->_originalImageSize) {
		delete this->_originalImageSize;
		this->_originalImageSize = nullptr;
	}

	for (auto trimIns : this->_trimInstructions) {
		delete trimIns.second;
	}

	this->_trimInstructions.clear();

	if (this->_trimedTexture) {
		SDL_DestroyTexture(this->_trimedTexture);
		this->_trimedTexture = NULL;
	}
}
