#include "GameContext.h"

GameContext::GameContext(SDL_Renderer* renderer)
{
	this->_renderer = renderer;
}

void GameContext::playScript(Script* script)
{
	for (int i = 0; i < script->getEvents().size(); i++) {
		printf("event [%i]: 0x%X\n", i, script->getEvents()[i]->action);
	}
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
				BackGroundMusic* backGroundMusic = new BackGroundMusic(debugString + currentScript->getEvents()[j]->data);
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

void GameContext::clear()
{
	for (int i = 0; i < this->_scripts.size(); i++) {
		this->_scripts[i]->free();
		//delete(this->_scripts[i]);
	}
	this->_scripts.clear();

	for (int i = 0; i < this->_voices.size(); i++) {
		this->_voices[i]->free();
		//delete(this->_voices[i]);
	}
	this->_voices.clear();

	for (int i = 0; i < this->_soundEffects.size(); i++) {
		this->_soundEffects[i]->free();
		//delete(this->_soundEffects[i]);
	}
	this->_soundEffects.clear();

	for (int i = 0; i < this->_backGroundMusics.size(); i++) {
		this->_backGroundMusics[i]->free();
		//delete(this->_backGroundMusics[i]);
	}
	this->_backGroundMusics.clear();

	for (int i = 0; i < this->_backGrounds.size(); i++) {
		this->_backGrounds[i]->free();
		//delete(this->_backGrounds[i]);
	}
	this->_backGrounds.clear();
}

void GameContext::setSystem(System* system)
{
	this->_system = system;
}

System* GameContext::getSystem()
{
	if (this->_system == nullptr) {
		printf("warning: system is nullptr\n");
	}
	return this->_system;
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

Script* GameContext::getScript(std::string path)
{
	for (int i = 0; i < this->_scripts.size(); i++) {
		if (this->_scripts[i]->getPath() == path) {
			return this->_scripts[i];
		}
	}
	return nullptr;
}

Voice* GameContext::getVoice(std::string path)
{
	for (int i = 0; i < this->_voices.size(); i++) {
		if (this->_voices[i]->getPath() == path) {
			return this->_voices[i];
		}
	}
	return nullptr;
}

SoundEffect* GameContext::getSoundEffect(std::string path)
{
	for (int i = 0; i < this->_soundEffects.size(); i++) {
		if (this->_soundEffects[i]->getPath() == path) {
			return this->_soundEffects[i];
		}
	}
	return nullptr;
}

BackGroundMusic* GameContext::getBackGroundMusic(std::string path)
{
	for (int i = 0; i < this->_backGroundMusics.size(); i++) {
		if (this->_backGroundMusics[i]->getPath() == path) {
			return this->_backGroundMusics[i];
		}
	}
	return nullptr;
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
