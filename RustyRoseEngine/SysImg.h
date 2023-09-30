#pragma once
#include <string>
#include <SDL_image.h>
#include <vector>
#include <fstream>

class SysImg
{
public:
	struct Button {
		int id;
		int x;
		int y;
		int w;
		int h;
	};

	struct OriginalSize {
		int x;
		int y;
	};

	SysImg(std::string path, SDL_Renderer* renderer);
	bool load();
	
	SDL_Texture* getTexture();
	void trimTexture(int id); //id = id of button to trim, 0 = all texture

	void setDestRect(SDL_Rect* rect);
	SDL_Rect* getDestRect();
	void addButtons(std::string path);
	SDL_Rect getButtonRect(int id);
	SDL_Rect getRealSizeOfImage();
	std::string getPath();
	void free();

private:
	std::string _path;
	SDL_Renderer* _renderer;

	SDL_Texture* _texture;
	SDL_Surface* _surface;
	std::vector<Button*> _buttons;
	OriginalSize* _originalSize;
	SDL_Rect* _destRect;

	Button* _getButton(int id);

};

