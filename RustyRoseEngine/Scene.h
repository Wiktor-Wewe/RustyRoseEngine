#pragma once
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <vector>
#include "BackGround.h"
#include "SysImg.h"
class Scene
{
public:
	Scene(SDL_Renderer* renderer);
	
	void setFont(TTF_Font* font);
	void draw();
	void clear(int layer); //-1 = all // -2 = text
	void addBackGround(BackGround* bg, int layer);
	void addSysImg(SysImg* sysimg, int layer);
	void addText(std::string text);
	void makeTexture();

private:
	SDL_Renderer* _renderer;
	SDL_Texture* _texture;
	TTF_Font* _font;

	std::vector<BackGround*> _backGround0;
	std::vector<BackGround*> _backGround1;
	std::vector<BackGround*> _backGround2;
	std::vector<SysImg*> _sysImg0;
	std::vector<SysImg*> _sysImg1;
	std::vector<SysImg*> _sysImg2;
	std::vector<std::string> _text;

	SDL_Texture* _makeText(std::string text, int& w, int& h);
};

