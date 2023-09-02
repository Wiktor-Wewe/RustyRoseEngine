#include "Voice.h"

Voice::Voice(std::string path)
{
	this->_path = path;
	this->_voice = new SoLoud::Wav();
}

void Voice::load()
{
	auto result = this->_voice->load(this->_path.c_str());
	if (result != SoLoud::SO_NO_ERROR) {
		printf("unable to load voice: %s\n", this->_path.c_str());
	}
}

void Voice::play(SoLoud::Soloud* soloud)
{
	if (this == nullptr) {
		printf("ERROR - Trying to play NULL in Voices\n");
		return;
	}

	this->_handle = soloud->play(*this->_voice);
	// add - unable to play voice: %s\n
}

void Voice::pause(SoLoud::Soloud* soloud)
{
	soloud->setPause(this->_handle, true);
}

void Voice::resume(SoLoud::Soloud* soloud)
{
	soloud->setPause(this->_handle, false);
}

void Voice::stop(SoLoud::Soloud* soloud)
{
	if (this == nullptr) {
		printf("ERROR - Trying to stop NULL in Voices\n");
		return;
	}

	soloud->stop(this->_handle);
}

void Voice::setSpeed(SoLoud::Soloud* soloud, float speed)
{
	soloud->setRelativePlaySpeed(this->_handle, speed);
}

void Voice::free()
{
	if (this == nullptr) {
		printf("ERROR - Trying to free NULL in Voices\n");
		return;
	}

	delete this->_voice;
	this->_voice = NULL;
}

std::string Voice::getPath()
{
	return this->_path;
}
