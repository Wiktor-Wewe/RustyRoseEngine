#pragma once
#include "SharedUtils.h"

class Jumps
{
public:
	struct Jump
	{
		int id = 0;
		int routeId = 0;
		std::string scriptName;
		int jumpToId = 0;
	};

	Jumps();
	bool load(std::string path);
	void setStart(std::string scriptName);
	void move(int playerOption);

	Jump* getCurrentJump();
	bool getStatus();

	~Jumps();

private:
	bool _status;

	std::vector<Jump*> _jumps;
	Jump* _currentJump;
	

};

