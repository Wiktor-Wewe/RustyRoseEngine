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
	this->_speed = 1;

	this->globalSE = new GlobalSE();
}

void SoundManager::add(std::string path, Type type)
{
	Sound* sound = nullptr;

	switch (type) {
	case Type::BackGroundMusicType: {
		auto bgm = new BackGroundMusic(path, this->_soloud, this->_speed, type);
		bgm->load();
		sound = bgm;
		} break;

	case Type::SoundEffectType: {
		auto se = new SoundEffect(path, this->_soloud, this->_speed, type);
		se->load();
		sound = se;
		} break;

	case Type::SoundType: {
		sound = new Sound(path, this->_soloud, this->_speed);
		sound->load();
		} break;

	case Type::VoiceType: {
		auto v = new Voice(path, this->_soloud, this->_speed, type);
		v->load();
		sound = v;
		} break;
	}

	if (sound) {
		this->_sounds.push_back(sound);
	}
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
			this->_deleteOnType(this->_sounds[i]);
			this->_sounds.erase(this->_sounds.begin() + i);
			return;
		}
	}
}

void SoundManager::loadAll()
{
	// i dont know if its needed anymore | if no -> delete | if yes -> add load depend on sound type
	for (auto sound : this->_sounds) {
		sound->load();
	}
}

void SoundManager::freeAll()
{
	// same as loadAll
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
		this->_deleteOnType(sound);
	}
	this->_sounds.clear();
}

SoundManager::~SoundManager()
{
	this->clear();
	delete this->globalSE;
	this->_soloud->deinit();
	delete this->_soloud;
}

void SoundManager::_deleteOnType(Sound* sound)
{
	switch (sound->getType()) {

	case Type::BackGroundMusicType: {
		auto bgm = (BackGroundMusic*)sound;
		delete bgm;
		} break;

	case Type::SoundEffectType: {
		auto se = (SoundEffect*)sound;
		delete se;
		} break;

	case Type::SoundType: {
		delete sound;
		} break;

	case Type::VoiceType: {
		auto v = (Voice*)sound;
		delete v;
		} break;
	}
}
