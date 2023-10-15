#include "SoundManager.h"

SoundManager::SoundManager()
{
	this->_soloud = new SoLoud::Soloud();
	this->_soloud->init();
}

void SoundManager::add(std::string path)
{
	Sound* sound = new Sound(path, this->_soloud, this->_speed);

	this->_sounds.push_back(sound);
}

void SoundManager::add(Sound* sound)
{
	sound->setSoLoud(this->_soloud);
	sound->setSpeed(this->_speed);
	this->_sounds.push_back(sound);
}

Sound* SoundManager::get(std::string path)
{
	for (auto sound : this->_sounds) {
		if (sound->getPath() == path) {
			return sound;
		}
	}

	return nullptr;
}

void SoundManager::remove(std::string path)
{
	for (int i = 0; i < this->_sounds.size(); i++) {
		if (this->_sounds[i]->getPath() == path) {
			this->_sounds[i]->stop();
			delete this->_sounds[i];
			this->_sounds.erase(this->_sounds.begin() + i);
			return;
		}
	}
}

void SoundManager::loadAll()
{
	for (auto sound : this->_sounds) {
		sound->load();
	}
}

void SoundManager::freeAll()
{
	for (auto sound : this->_sounds) {
		sound->free();
	}
}

void SoundManager::pauseAll()
{
	for (auto sound : this->_sounds) {
		sound->pause();
	}
}

void SoundManager::resumeAll()
{
	for (auto sound : this->_sounds) {
		sound->resume();
	}
}

void SoundManager::setSpeed(double speed)
{
	if (this->_speed != speed) {
		this->_speed = speed;
		for (auto sound : this->_sounds) {
			sound->setSpeed(speed);
		}
	}
}

void SoundManager::clear()
{
	for (auto sound : this->_sounds) {
		sound->stop();
		delete sound;
	}
	this->_sounds.clear();
}

SoundManager::~SoundManager()
{
	this->clear();
	this->_soloud->deinit();
	delete this->_soloud;
}
