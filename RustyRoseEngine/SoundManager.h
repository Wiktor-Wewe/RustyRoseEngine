#pragma once
#include "SharedUtils.h"
#include "Sound.h"
#include "BackGroundMusic.h"
#include "SoundEffect.h"
#include "Voice.h"
#include "IniFile.h"

class SoundManager
{
public:
	enum Type {
		SoundType,
		BackGroundMusicType,
		SoundEffectType,
		VoiceType
	};

	struct GlobalSE {
		GlobalSE() {
			this->Cancel = nullptr;
			this->Click = nullptr;
			this->Open = nullptr;
			this->Select = nullptr;
			this->Up = nullptr;
			this->View = nullptr;
		}

		Sound* Cancel;
		Sound* Click;
		Sound* Open;
		Sound* Select;
		Sound* Up;
		Sound* View;

		~GlobalSE() {
			if (this->Cancel) delete this->Cancel;
			if (this->Click) delete this->Click;
			if (this->Open) delete this->Open;
			if (this->Select) delete this->Select;
			if (this->Up) delete this->Up;
			if (this->View) delete this->Up;
		}
	};

	GlobalSE* globalSE;
	void setGlobalSE(IniFile* ini);

	SoundManager();
	
	void add(std::string path, Type type = Type::SoundType);

	Sound* get(std::string path);
	void remove(std::string path);
	
	void loadAll();
	void freeAll();

	void pauseAll();
	void resumeAll();

	void setSpeed(double speed);

	void clear();

	~SoundManager();

private:
	SoLoud::Soloud* _soloud;
	double _speed;

	std::vector<Sound*> _sounds;
	void _deleteOnType(Sound* sound);
};