#include "BackGroundMusic.h"

BackGroundMusic::BackGroundMusic(std::string path, SoLoud::Soloud* soloud, double speed) : Sound(path, soloud, speed)
{
	this->_initDone = false;
	this->_waveLoop = new SoLoud::Wav();
	this->_pathInit = RRE_NormalizePath(path + "_INT.OGG");
	this->_pathLoop = RRE_NormalizePath(path + "_LOOP.OGG");
}

void BackGroundMusic::load()
{
	if (this == NULL) {
		printf("ERROR - Trying to load NULL in sound\n");
		return;
	}

	auto result1 = this->_wave->load(this->_pathInit.c_str());
	if (result1 != SoLoud::SO_NO_ERROR) {
		printf("unable to load background muisc int: %s\n", this->_path.c_str());
		printf("its possible that this file just not exists\n");
	}

	auto result2 = this->_waveLoop->load(this->_pathLoop.c_str());
	if (result2 != SoLoud::SO_NO_ERROR) {
		printf("unable to load background muisc loop: %s\n", this->_path.c_str());
	}
}

void BackGroundMusic::play()
{
	if (this == NULL) {
		printf("ERROR - Trying to play NULL in sound\n");
		return;
	}

	if (this->_initDone) {
		this->_handle = this->_soloud->play(*this->_waveLoop);
		this->_soloud->setLooping(this->_handle, true);
	}
	else {
		this->_handle = this->_soloud->play(*this->_wave);
	}
}

bool BackGroundMusic::isInitDone()
{
	if (this == NULL) {
		printf("ERROR - Trying to check init status in NULL in sound\n");
		return false;
	}

	if (this->_soloud->isValidVoiceHandle(this->_handle)) {
		this->_initDone = true;
	}
	return this->_initDone;
}

void BackGroundMusic::pause()
{
	if (this == NULL) {
		printf("ERROR - Trying to pause NULL in sound\n");
		return;
	}

	this->_soloud->setPause(this->_handle, true);
}

void BackGroundMusic::resume()
{
	if (this == NULL) {
		printf("ERROR - Trying to pause NULL in sound\n");
		return;
	}

	this->_soloud->setPause(this->_handle, false);
}

void BackGroundMusic::stop()
{
	if (this == NULL) {
		printf("ERROR - Trying to stop NULL in sound\n");
		return;
	}

	this->_soloud->stop(this->_handle);
	this->_initDone = false;
}

void BackGroundMusic::setSpeed(float speed)
{
	if (this == NULL) {
		printf("ERROR - Trying to set speed in NULL in sound\n");
		return;
	}

	this->_soloud->setRelativePlaySpeed(this->_handle, speed);
}

void BackGroundMusic::free()
{
	if (this->_wave) {
		delete this->_wave;
	}

	if (this->_waveLoop) {
		delete this->_waveLoop;
	}

	this->_initDone = false;
}

BackGroundMusic::~BackGroundMusic()
{
	if (this->_waveLoop) {
		delete this->_waveLoop;
	}
}
