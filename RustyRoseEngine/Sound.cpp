#include "Sound.h"

Sound::Sound(std::string path, SoLoud::Soloud* soloud, double speed, int type)
{
	this->_type = type;
	this->_speed = speed;
	this->_path = path;
	this->_soloud = soloud;
	this->_wave = new SoLoud::Wav();
}

int Sound::getType()
{
	return this->_type;
}

void Sound::load()
{
	if (this == NULL) {
		RRE_LogError("ERROR - Trying to load NULL in sound");
		return;
	}

	auto result = this->_wave->load(this->_path.c_str());
	if (result != SoLoud::SO_NO_ERROR) {
		RRE_LogError("unable to load sound: \n" + this->_path);
	}
}

void Sound::play()
{
	if (this == NULL) {
		RRE_LogError("ERROR - Trying to play NULL in sound\n");
		return;
	}

	this->_handle = this->_soloud->play(*this->_wave);
	this->_soloud->setRelativePlaySpeed(this->_handle, this->_speed);

	if (this->_soloud->isValidVoiceHandle(this->_handle) == false) {
		RRE_LogError("unable to play sound: \n" + this->_path);
	}
}

void Sound::pause()
{
	if (this == NULL) {
		RRE_LogError("ERROR - Trying to pause NULL in sound");
		return;
	}

	this->_soloud->setPause(this->_handle, true);
}

void Sound::resume()
{
	if (this == NULL) {
		RRE_LogError("ERROR - Trying to resume NULL in sound");
		return;
	}

	this->_soloud->setPause(this->_handle, false);
}

void Sound::stop()
{
	if (this == NULL) {
		RRE_LogError("ERROR - Trying to stop NULL in sound\n");
		return;
	}

	this->_soloud->stop(this->_handle);
}

void Sound::setSpeed(float speed)
{
	if (this == NULL) {
		RRE_LogError("ERROR - Trying to setSpeed NULL in sound");
		return;
	}

	this->_speed = speed;
	this->_soloud->setRelativePlaySpeed(this->_handle, speed);
}

void Sound::free()
{
	delete this->_wave;
	this->_wave = new SoLoud::Wav();
}

std::string Sound::getPath()
{
	return this->_path;
}

Sound::~Sound()
{
	if (this->_wave) {
		delete this->_wave;
	}
}
