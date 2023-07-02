#include "Episode.h"

Episode::Episode(std::string path)
{
	this->_path = path;
}

bool Episode::load()
{
	std::fstream file;
	file.open(this->_path.c_str(), std::ios::in);
	if (!file.good()) {
		printf("unable to load file: %s\n", this->_path.c_str());
		return false;
	}

	std::string buff;
	std::vector<std::string> list;
	while (!file.eof()) {
		std::getline(file, buff);
		list.push_back(buff);
	}
	file.close();

	//simple filr for test

	for (int i = 0; i < list.size(); i++) {
		if (list[i].find(".OGG") != std::string::npos && list[i].find("Se00")!=std::string::npos) {
			// sound effects
			this->_soundEffecfs.push_back(new SoundEffect(list[i]));
		}
		else if (list[i].find(".OGG") != std::string::npos && list[i].find("Voice00")!=std::string::npos) {
			// voice
			this->_voices.push_back(new Voice(list[i]));
		}
		else if (list[i].find(".ORS") != std::string::npos) {
			// script
			this->_scripts.push_back(new Script(list[i]));
		}
		else if (list[i].find(".PNG") != std::string::npos && list[i].find("Voice00") != std::string::npos) {
			// backgrund

		}
	}

	//

	return true;
}
