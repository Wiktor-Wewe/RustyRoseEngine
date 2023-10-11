#pragma once
#include "SharedUtils.h"

class Image
{
public:
	Image(SDL_Renderer* renderer, RRW_ScreenSize* screenSize, std::string path);
	void load();
	void loadTrimInstructions(std::string path);
	void free();
	void setTextureToTrim(std::string path);
	void trimTexture(int id); // if 0 -> free trimedTexture and set default

	std::string getPath();
	SDL_Texture* getTexture();
	SDL_Texture* getTrimedTexture();
	SDL_Rect* getRectOfTrim(int id);

	~Image();

protected:
	SDL_Renderer* _renderer;
	RRW_ScreenSize* _screenSize;

	std::string _path;
	SDL_Texture* _texture;

	// buttons
	SDL_Texture* _textureToTrim; // default: _textureToTrim = _texture
	RRW_ScreenSize* _originalImageSize; // original size of image
	std::unordered_map<int, SDL_Rect*> _trimInstructions; // int = id, Rect* = position
	SDL_Texture* _trimedTexture; // element of original texture
};

