#include "Sound.h"

Sound::Sound(std::string path, SoLoud::Soloud* soloud, double speed)
{
	this->_speed = speed;
	this->_path = path;
	this->_soloud = soloud;
	this->_wave = new SoLoud::Wav();
}

void Sound::load()
{
	if (this == NULL) {
		printf("ERROR - Trying to load NULL in sound\n");
		return;
	}

	auto result = this->_wave->load(this->_path.c_str());
	if (result != SoLoud::SO_NO_ERROR) {
		printf("unable to load sound: %s\n", this->_path.c_str());
	}
}

void Sound::play()
{
	if (this == NULL) {
		printf("ERROR - Trying to play NULL in sound\n");
		return;
	}

	this->_handle = this->_soloud->play(*this->_wave);
	this->_soloud->setRelativePlaySpeed(this->_handle, this->_speed);
	// add - unable to play voice: %s\n
}

void Sound::pause()
{
	if (this == NULL) {
		printf("ERROR - Trying to pause NULL in sound\n");
		return;
	}

	this->_soloud->setPause(this->_handle, true);
}

void Sound::resume()
{
	if (this == NULL) {
		printf("ERROR - Trying to resume NULL in sound\n");
		return;
	}

	this->_soloud->setPause(this->_handle, false);
}

void Sound::stop()
{
	if (this == NULL) {
		printf("ERROR - Trying to stop NULL in sound\n");
		return;
	}

	this->_soloud->stop(this->_handle);
}

void Sound::setSpeed(float speed)
{
	if (this == NULL) {
		printf("ERROR - Trying to setSpeed NULL in sound\n");
		return;
	}

	this->_speed = speed;
	this->_soloud->setRelativePlaySpeed(this->_handle, speed);
}

void Sound::free()
{
	delete this->_wave;
	this->_wave = nullptr;
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
