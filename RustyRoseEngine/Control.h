#pragma once
#include <vector>
class Control
{
public:

	enum Action {
		quit = 0,
		next = 1,
		pause = 2, 

		left = 3,
		right = 4,
		up = 5,
		down = 6,
		
		enter = 7,
		back = 8
	};

	Control();
	void add(Action x);
	void clear();
	bool check(Action x);
	bool isAction();

private:
	bool _status;
	std::vector<int> _pressedKeys;

};

