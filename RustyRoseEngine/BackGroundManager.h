#pragma once
#include "SharedUtils.h"
#include <RustyRenderWindow.h>
#include "BackGround.h"
#include "Script.h"

class BackGroundManager
{
public:
	BackGroundManager(SDL_Renderer* renderer, RustyScene* scene, RRW_ScreenSize* screenSize);

	void setEventsStateLists(Script::EventsStateLists* esl);
	void addBG(std::string path);
	void setBG(std::string path);
	void freeBG(std::string path);

	void setShortName(std::string shortName);
	void resetShortName(std::string shortName = std::string());
	void setAnimationStatus(bool status);
	void handleAnimation();
	void clear();

	~BackGroundManager();

private:
	SDL_Renderer* _renderer;
	RustyScene* _scene;
	RRW_ScreenSize* _screenSize;
	Script::EventsStateLists* _eventsStateLists;

	bool _isAnimation;
	std::vector<std::string> _names;
	std::string _shortName;
	BackGround* _currentBg;
	std::vector<BackGround*> _backgrounds;
};

