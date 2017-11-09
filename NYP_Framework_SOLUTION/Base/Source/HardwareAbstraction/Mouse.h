#pragma once

#include "Controller.h"

class Mouse :
	public Controller
{
public:
	Mouse();
	virtual ~Mouse();
	// Create this controller
	virtual bool Create(CPlayerInfo* thePlayerInfo = NULL);
	// Read from the controller
	virtual int Read(const float deltaTime);
};

