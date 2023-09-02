#include "SoundEffect.h"

SoundEffect::SoundEffect(std::string path)
{
	this->_path = path;
	this->_soundEffect = new SoLoud::Wav();
}

void SoundEffect::load()
{
	if (this->_soundEffect == NULL) { // <- dont know why, but its work so dont touch it :)
		this->_soundEffect = new SoLoud::Wav();
	}

	auto result = this->_soundEffect->load(this->_path.c_str());
	if (result != SoLoud::SO_NO_ERROR) {
		printf("unable to load sound effect: %s\n", this->_path.c_str());
	}
}

void SoundEffect::play(SoLoud::Soloud* soloud)
{
	if (this == nullptr) {
		printf("ERROR - Trying to play NULL in SoundEffects\n");
		return;
	}

	if (this->_soundEffect == NULL) {
		this->_soundEffect = new SoLoud::Wav();
	}

	this->_handle = soloud->play(*this->_soundEffect);
	// add - unable to play sound effect: %s\n
}

void SoundEffect::pause(SoLoud::Soloud* soloud)
{
	soloud->setPause(this->_handle, true);
}

void SoundEffect::resume(SoLoud::Soloud* soloud)
{
	soloud->setPause(this->_handle, false);
}

void SoundEffect::stop(SoLoud::Soloud* soloud)
{
	if (this == nullptr) {
		printf("ERROR - Trying to stop NULL in SoundEffects\n");
		return;
	}

	soloud->stop(this->_handle);
}

void SoundEffect::setSpeed(SoLoud::Soloud* soloud, float speed)
{
	soloud->setRelativePlaySpeed(this->_handle, speed);
}

void SoundEffect::free()
{
	if (this == nullptr) {
		printf("ERROR - Trying to free NULL in SoundEffects\n");
		return;
	}

	delete this->_soundEffect;
	this->_soundEffect = NULL;
}

std::string SoundEffect::getPath()
{
	return this->_path;
}
