#pragma once

#include "Controller.h"

#include <fstream>

class Keyboard :
	public Controller
{
private:
    unsigned int KeyList[NUM_CONRTOLLER];
public:
	Keyboard();
	virtual ~Keyboard();
	// Create this controller
	virtual bool Create(CPlayerInfo* thePlayerInfo = NULL);
	// Read from the controller
    virtual bool Load(std::string _filePath);
	virtual int Read(const float deltaTime);
	virtual void Update(double dt);
};

