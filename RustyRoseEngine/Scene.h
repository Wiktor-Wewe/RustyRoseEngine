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

	void setAnimationShortName(std::string shortName);
	void setAnimationShortNameToDefalut();
	void setAnimationShortNameToDefaultIfName(std::string shortName);

	BackGround* getLastBackGround(int layer);
	
	void clear(int layer); //-1 = all // -2 = text // -3 = video frame but only pointer
	
	void addBackGround(BackGround* bg, int layer);
	void addSysImg(SysImg* sysimg, int layer);
	void addVideoFrame(SDL_Texture* frame);
	void addText(std::string text);
	void addPathOption(std::string option);
	void setPathOptionByDirection(int dy);
	void setPathOptionByIndex(int index);
	bool isPathOptionSet();
	int getPathOption();
	void clearPathOption();
	
	void removeText(std::string text);
	void removeBackGround(BackGround* backGround, int layer); // -1 all

private:
	SDL_Renderer* _renderer;
	TTF_Font* _font;

	SDL_mutex* _textMutex;

	std::string _shortName;

	// backgrounds
	std::vector<BackGround*> _backGround0;
	std::vector<BackGround*> _backGround1;
	std::vector<BackGround*> _backGround2;

	// system images, menu, etc.
	std::vector<SysImg*> _sysImg0;
	std::vector<SysImg*> _sysImg1;
	std::vector<SysImg*> _sysImg2;

	// subtitles
	std::vector<std::string> _text;
	std::vector<SDL_Texture*> _textTexture;
	std::vector<SDL_Rect*> _textRect;

	// select path options in game
	int _selectedOptionIndex;
	std::vector<std::string> _pathOptions;
	std::vector<SDL_Texture*> _pathOptionsTexture;
	std::vector<SDL_Rect*> _pathOptionsRect;


	SDL_Texture* _videoFrame;

	SDL_Texture* _makeText(std::string text, int& w, int& h, SDL_Color textColor = { 255, 255, 255, 255 });
	void _tryRemoveBg(BackGround* bg, std::vector<BackGround*>& list);
};

