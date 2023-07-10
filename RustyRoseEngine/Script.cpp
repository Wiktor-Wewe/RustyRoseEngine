#include "Script.h"

Script::Script(std::string path)
{
	this->_path = path;
	
	std::fstream file;
	file.open(this->_path.c_str(), std::ios::in || std::ios::binary);

	if (!file.good()) {
		printf("unable to load script file: %s\n", this->_path.c_str());
		return;
	}

	if (file.good()) {
		this->_loadScript(&file);
	}
	else {
		printf("file: %s is not good\n", this->_path.c_str());
	}

	file.close();
}

bool Script::_loadScript(std::fstream* file)
{
	uint32_t buff32; uint16_t buff16; uint8_t buff8; std::string buffS;
	file->read(&buffS[0], 0x8);
	
	if (buffS != "RoseScri") {
		printf("Error - Header is missing: %s\n", this->_path.c_str());
		return false;
	}

	file->read(reinterpret_cast<char*>(&buff32), sizeof(buff32));
	int size = buff32;

	while (file->tellg() != size) {
		file->read(reinterpret_cast<char*>(&buff16), sizeof(buff16));
		switch (buff16) {
		case 0xCC01:
			this->_loadSkipFrame(file);
			break;

		case 0xCC02:
			this->_loadPlayBgm(file);
			break;

		case 0xCC03:
			this->_loadCreateBG(file);
			break;

		case 0xCC04:
			this->_loadPrintText(file);
			break;

		case 0xCC05:
			this->_loadPlayVoice(file);
			break;

		case 0xCC06:
			this->_loadPlaySe(file);
			break;

		case 0xCC07:
			this->_loadNext(file);
			break;

		case 0xCC08:
			this->_loadPlayMovie(file);
			break;

		case 0xCC09:
			this->_loadBlackFade(file);
			break;

		case 0xCC0A:
			this->_loadWhiteFade(file);
			break;

		case 0xCC0B:
			this->_loadSetSELECT(file);
			break;

		case 0xCC0C:
			this->_loadEndBGM(file);
			break;

		case 0xCC0D:
			this->_loadEndRoll(file);
			break;

		case 0xCC0E:
			this->_loadMoveSom(file);
			break;

		default:
			printf("ERROR - unknown command: %u\n", buff16);
			return false;
			break;
		}
	}

	return true;
}

