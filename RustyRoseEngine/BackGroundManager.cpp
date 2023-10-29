#include "BackGroundManager.h"

BackGroundManager::BackGroundManager(SDL_Renderer* renderer, RustyScene* scene, RRW_ScreenSize* screenSize)
{
	this->_renderer = renderer;
	this->_scene = scene;
	this->_screenSize = screenSize;
	this->_eventsStateLists = nullptr;

	this->_isAnimation = false;
	this->_currentBg = nullptr;
}

void BackGroundManager::setEventsStateLists(Script::EventsStateLists* eventsStateLists)
{
	this->_names.clear();
	this->_isAnimation = false;

	this->_eventsStateLists = eventsStateLists;
	for (auto event : this->_eventsStateLists->toLoad) {
		if (event->type == Script::EventType::PlayVoice) {
			if (std::find(this->_names.begin(), this->_names.end(), event->shortName) == this->_names.end()) {
				this->_names.push_back(event->shortName);
			}
		}
	}
}

void BackGroundManager::addBG(std::string path)
{
	auto bg = new BackGround(this->_renderer, this->_screenSize, path);
	bg->load();

	for (auto name : this->_names) {
		bg->tryLoadAnimation(name);
	}

	this->_backgrounds.push_back(bg);
}

void BackGroundManager::setBG(std::string path)
{
	if (this->_currentBg != nullptr) {
		this->_scene->removeImage(this->_currentBg->getPath());
	}
	this->_scene->removeImage("anim");
	
	for (auto bg : this->_backgrounds) {
		if (bg->getPath() == path) {
			this->_currentBg = bg;
			this->_scene->addImage(path, bg->getTexture(), NULL);
			return;
		}
	}

	this->_currentBg = nullptr;
}

void BackGroundManager::freeBG(std::string path)
{
	this->_scene->removeImage("anim");
	this->_scene->removeImage(path);

	for (int i = 0; i < this->_backgrounds.size(); i++) {
		if (this->_backgrounds[i]->getPath() == path) {
			delete this->_backgrounds[i];
			if (this->_currentBg == this->_backgrounds[i]) {
				this->_currentBg = nullptr;
			}
			this->_backgrounds.erase(this->_backgrounds.begin() + i);
			return;
		}
	}
}

void BackGroundManager::setShortName(std::string shortName)
{
	this->_shortName = shortName;
}

void BackGroundManager::resetShortName(std::string shortName)
{
	if (shortName.empty()) {
		this->_shortName.clear();
	}

	if (this->_shortName == shortName) {
		this->_shortName.clear();
		return;
	}
}

void BackGroundManager::setAnimationStatus(bool status)
{
	this->_isAnimation = status;
}

void BackGroundManager::handleAnimation()
{
	if (this->_currentBg == nullptr) {
		return;
	}

	this->_scene->removeImage("anim");

	if (this->_isAnimation == false) {
		return;
	}

	for (auto event : this->_eventsStateLists->inProgress) {
		if (event->type == Script::EventType::PlayMovie || event->type == Script::EventType::EndRoll) {
			return;
		}
	}

	this->_scene->addImage("anim", this->_currentBg->getAnimation(this->_shortName), NULL, 1);
}

void BackGroundManager::clear()
{
	this->_currentBg = nullptr;

	for (auto& bg : this->_backgrounds) {
		if(bg) delete bg;
	}

	this->_backgrounds.clear();
}

BackGroundManager::~BackGroundManager()
{
	this->clear();
}
