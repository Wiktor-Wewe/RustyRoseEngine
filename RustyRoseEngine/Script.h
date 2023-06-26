#pragma once
class Script
{
public:
	struct Time
	{
		unsigned int minute;
		unsigned int second;
		unsigned int millisecond;
	};

private:
	char* _fileName;
	Time _skipFRAME;


};

