#include "BackGroundManager.h"

BackGroundManager::BackGroundManager(SDL_Renderer* renderer, RustyScene* scene, RRW_ScreenSize* screenSize)
{
	this->_renderer = renderer;
	this->_scene = scene;
	this->_screenSize = screenSize;
	this->_eventsStateLists = nullptr;

	this->_isAnimation = false;
	this->_currentBackGround = nullptr;
	this->_futureBackGround = nullptr;
}

void BackGroundManager::setEventsStateLists(Script::EventsStateLists* eventsStateLists)
{
	this->_eventsStateLists = eventsStateLists;
	this->_names.clear();
	for (auto event : this->_eventsStateLists->toLoad) {
		if (event->type == Script::EventType::PlayVoice) {
			if (std::find(this->_names.begin(), this->_names.end(), event->shortName) == this->_names.end()) {
				this->_names.push_back(event->shortName);
			}
		}
	}
}

void BackGroundManager::setFutureBg(std::string path)
{
	auto bg = new BackGround(this->_renderer, this->_screenSize, path);
	this->_futureBackGround = bg;
	this->_futureBackGround->load();
	for (auto name : this->_names) {
		bg->tryLoadAnimation(name);
	}
}

void BackGroundManager::swapBg()
{
	this->_scene->removeImage("anim");

	if (this->_currentBackGround) {
		this->_scene->removeImage(this->_currentBackGround->getPath());
		delete this->_currentBackGround;
	}

	this->_currentBackGround = this->_futureBackGround;
	this->_futureBackGround = nullptr;
	if (this->_currentBackGround) {
		this->_scene->addImage(this->_currentBackGround->getPath(), this->_currentBackGround->getTexture(), NULL);
	}
}

void BackGroundManager::setShortName(std::string shortName)
{
	this->_shortName = shortName;
}

void BackGroundManager::resetShortName(std::string shortName)
{
	if (shortName.empty()) {
		this->_shortName = shortName;
		return;
	}

	if (this->_shortName == shortName) {
		this->_shortName.clear();
	}
}

void BackGroundManager::startAnimation()
{
	this->_isAnimation = true;
}

void BackGroundManager::stopAnimation()
{
	this->_isAnimation = false;
}

void BackGroundManager::handleAnimation()
{
	if (this->_isAnimation == false) {
		return;
	}

	if (this->_currentBackGround == nullptr) {
		return;
	}

	for (auto e : this->_eventsStateLists->inProgress) {
		if (e->type == Script::EventType::PlayMovie || e->type == Script::EventType::EndRoll) {
			this->_scene->removeImage("anim");
			return;
		}
	}

	this->_scene->removeImage("anim");
	this->_scene->addImage("anim", this->_currentBackGround->getAnimation(this->_shortName), NULL);
}

void BackGroundManager::free()
{
	if (this->_currentBackGround) {
		delete this->_currentBackGround;
		this->_currentBackGround = nullptr;
	}

	if (this->_futureBackGround) {
		delete this->_futureBackGround;
		this->_futureBackGround = nullptr;
	}
}

BackGroundManager::~BackGroundManager()
{
	this->free();
}
