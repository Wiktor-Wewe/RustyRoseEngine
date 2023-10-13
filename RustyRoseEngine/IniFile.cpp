#include "IniFile.h"

IniFile::IniFile()
{
	this->_info["title"] = "RustyRoseEngine: SchoolDaysHQ";
	this->_info["debugString"] = std::string(); // default = no debugString
	this->_info["startScript"] = "00/00-00-A00";
	this->_info["mainPath"] = "./data/";
	this->_info["windowWidth"] = "1280";
	this->_info["windowHeight"] = "720";
	this->_info["pathToJump"] = "./data/boatJumps.rosej";
	this->_info["pathToSave"] = "./Save/";
	this->_info["pathToFont"] = "./data/ARIAL.TTF";
}

void IniFile::loadFile(std::string path)
{
	std::string line;
	std::fstream file(path.c_str(), std::ios::in);

	if (file.good() == false) {
		printf("unable to open ini file: %s\n", path.c_str());
		return;
	}

	while (file.eof() == false) {

		std::getline(file, line);
		auto pair = RRE_Split(line, '=');

		if (pair.size() == 2) { // sould be key and value after split
			if (this->_info.find(pair[0]) != this->_info.end()) { // check if key is already in map
				if (pair[0] == "windowWidth" || pair[0] == "windowHeight") { // check if this string contain int
					if (this->_isGoodToConvert(pair[1])) { // check if string is good to convert to int
						this->_info[pair[0]] = pair[1]; // add it
					}
				}
				else {
					this->_info[pair[0]] = pair[1]; // just add
				}
			}
		}

	}

	file.close();
}

std::string IniFile::getTitle()
{
	return this->_info["title"];
}

std::string IniFile::getDebugString()
{
	return this->_info["debugString"];
}

std::string IniFile::getStartScript()
{
	return this->_info["startScript"];
}

std::string IniFile::getMainPath()
{
	return this->_info["mainPath"];
}

int IniFile::getWindowWidth()
{
	return std::stoi(this->_info["windowWidth"]);
}

int IniFile::getWindowHeight()
{
	return std::stoi(this->_info["windowHeight"]);
}

std::string IniFile::getPathToJump()
{
	return this->_info["pathToJump"];
}

std::string IniFile::getPathToSave()
{
	return this->_info["pathToSave"];
}

std::string IniFile::getPathToFont()
{
	return this->_info["pathToFont"];
}

bool IniFile::_isGoodToConvert(std::string x)
{
	for (auto y : x) {
		if (y < 48 || y > 57) { // check if char in string is in range <48,57>
			return false;
		}
	}
	return true;
}
