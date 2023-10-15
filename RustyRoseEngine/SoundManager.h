#pragma once
#include "SharedUtils.h"
#include "Sound.h"
#include "IniFile.h"

class SoundManager
{
public:
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
	
	void add(std::string path);
	void add(Sound* sound);

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
};

