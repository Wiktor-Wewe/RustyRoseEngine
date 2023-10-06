#pragma once
#include "SharedUtils.h"

class IniFile
{
public:
	IniFile();
	void loadFile(std::string path);

	std::string getDebugString();
	std::string getMainPath();
	int getWindowWidth();
	int getWindowHeight();
	std::string getPathToJump();
	std::string getPathToSave();
	std::string getPathToFont();


private:
	/*
		- in _info -
		debugString
		mainPath
		windowWidth
		windowHeight
		pathToJump
		pathToSave
		pathToFont
	*/
	std::unordered_map<std::string, std::string> _info;

	bool _isGoodToConvert(std::string x);

};

