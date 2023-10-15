#pragma once
#include "SharedUtils.h"

class IniFile
{
public:
	IniFile();
	void loadFile(std::string path);

	std::string get(std::string key);

	std::string getTitle();
	std::string getDebugString();
	std::string getStartScript();
	std::string getMainPath();
	int getWindowWidth();
	int getWindowHeight();
	std::string getPathToJump();
	std::string getPathToSave();
	std::string getPathToFont();


private:
	std::unordered_map<std::string, std::string> _info;

	bool _isGoodToConvert(std::string x);

};

