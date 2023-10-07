#pragma once
#include "SharedUtils.h"

class Image
{
public:
	Image(SDL_Renderer* renderer, RRW_ScreenSize* screenSize, std::string path);
	virtual void load();
	virtual void loadTrimInstructions(std::string path);
	virtual void free();
	virtual void setTextureToTrim(std::string path);
	virtual void trimTexture(int id); // if 0 -> free trimedTexture and set default

	virtual std::string getPath();
	virtual SDL_Texture* getTexture();
	virtual SDL_Texture* getTrimedTexture();
	virtual SDL_Rect* getRectOfTrim(int id);

	virtual ~Image();

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

