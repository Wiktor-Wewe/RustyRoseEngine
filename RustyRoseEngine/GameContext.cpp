#include "GameContext.h"

GameContext::GameContext(SDL_Renderer* renderer)
{
	this->_renderer = renderer;
}

void GameContext::addScript(Script* script)
{
	this->_scripts.push_back(script);
}

bool GameContext::loadContentFromScripts()
{
	std::string debugString = "C:\\Users\\Wiktor\\source\\repos\\RustyRoseEngine\\x64\\Debug\\data\\";

	Script* currentScript = nullptr;
	for (int i = 0; i < this->_scripts.size(); i++) {
		currentScript = this->_scripts[i];
		for (int j = 0; j < currentScript->getEvents().size(); j++) {
			switch (currentScript->getEvents()[j]->action)
			{
			case 0xCC05: { // voice
				Voice* voice = new Voice(debugString + currentScript->getEvents()[j]->data + ".OGG");
				this->_voices.push_back(voice);
				}
				break;

			case 0xCC06: { // se
				SoundEffect* soundEffect = new SoundEffect(debugString + currentScript->getEvents()[j]->data + ".OGG");
				this->_soundEffects.push_back(soundEffect);
				}
				break;

			case 0xCC02: { // bgm
				BackGroundMusic* backGroundMusic = new BackGroundMusic(debugString + currentScript->getEvents()[j]->data + ".OGG");
				this->_backGroundMusics.push_back(backGroundMusic);
				}
				break;

			case 0xCC03: { //bg
				BackGround* backGround = new BackGround(this->_renderer, debugString + currentScript->getEvents()[j]->data);
				this->_loadAnimationForBackGround(currentScript, backGround, j);
				this->_backGrounds.push_back(backGround);
				}
				break;
			}
		}
	}

	return true;
}

void GameContext::_loadAnimationForBackGround(Script* script, BackGround* backGround, int currentEvent)
{
	currentEvent++;
	for (int i = currentEvent; i < script->getEvents().size(); i++) {
		if (script->getEvents()[i]->action == 0xCC03) {
			return;
		}

		if (!script->getEvents()[i]->shortName.empty() && script->getEvents()[i]->shortName != "xxx") {
			backGround->tryLoadAnimation(script->getEvents()[i]->shortName);
		}
	}
}

BackGround* GameContext::getBackGround(std::string path)
{
	for (int i = 0; i < this->_backGrounds.size(); i++) {
		if (this->_backGrounds[i]->getPath() == path) {
			return this->_backGrounds[i];
		}
	}
	return nullptr;
}
