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
	
	SDL_Texture* getTexture();
	void trimTexture(int id); //id = id of button to trim, 0 = all texture

	void addButtons(std::string path);
	std::string getPath();
	void free();

private:
	std::string _path;
	SDL_Renderer* _renderer;

	SDL_Texture* _texture;
	SDL_Surface* _surface;
	std::vector<Button*> _buttons;
	OriginalSize* _originalSize;

	bool _load();
	Button* _getButton(int id);

};

