#include "Control.h"

Control::Control()
{
	this->_status = false;
}

void Control::add(Action x)
{
	this->_status = true;
	this->_pressedKeys.push_back(x);
}

void Control::clear()
{
	this->_status = false;
	this->_pressedKeys.clear();
}

bool Control::check(Action x)
{
	for (int i = 0; i < this->_pressedKeys.size(); i++) {
		if (this->_pressedKeys[i] == x) {
			return true;
		}
	}
	return false;
}

bool Control::isAction()
{
	return this->_status;
}
