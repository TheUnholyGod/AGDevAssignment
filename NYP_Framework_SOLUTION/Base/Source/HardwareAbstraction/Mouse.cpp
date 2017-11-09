#include "Mouse.h"
#include <iostream>
using namespace std;

#include "MouseController.h"
#include "../PlayerInfo/PlayerInfo.h"

const bool _CONTROLLER_MOUSE_DEBUG = false;

Mouse::Mouse()
{
}


Mouse::~Mouse()
{
}

// Create this controller
bool Mouse::Create(CPlayerInfo* thePlayerInfo)
{
	Controller::Create(thePlayerInfo);
	if (_CONTROLLER_MOUSE_DEBUG)
		cout << "Mouse::Create()" << endl;

	return false;
}


// Read from the controller
int Mouse::Read(const float deltaTime)
{
	Controller::Read(deltaTime);
	if (_CONTROLLER_MOUSE_DEBUG)
		cout << "Mouse::Read()" << endl;

	return 0;
}
