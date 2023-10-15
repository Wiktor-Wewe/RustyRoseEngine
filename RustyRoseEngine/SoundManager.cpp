#include "SoundManager.h"

void SoundManager::setGlobalSE(IniFile* ini)
{
	this->globalSE->Cancel = new Sound(RRE_NormalizePath(ini->getDebugString() + ini->get("secancel")), this->_soloud, 1);
	this->globalSE->Cancel->load();

	this->globalSE->Click = new Sound(RRE_NormalizePath(ini->getDebugString() + ini->get("seclick")), this->_soloud, 1);
	this->globalSE->Click->load();

	this->globalSE->Open = new Sound(RRE_NormalizePath(ini->getDebugString() + ini->get("seopen")), this->_soloud, 1);
	this->globalSE->Open->load();

	this->globalSE->Select = new Sound(RRE_NormalizePath(ini->getDebugString() + ini->get("seselect")), this->_soloud, 1);
	this->globalSE->Select->load();

	this->globalSE->Up = new Sound(RRE_NormalizePath(ini->getDebugString() + ini->get("seup")), this->_soloud, 1);
	this->globalSE->Up->load();

	this->globalSE->View = new Sound(RRE_NormalizePath(ini->getDebugString() + ini->get("seview")), this->_soloud, 1);
	this->globalSE->View->load();
}

SoundManager::SoundManager()
{
	this->_soloud = new SoLoud::Soloud();
	this->_soloud->init();

	this->globalSE = new GlobalSE();
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
