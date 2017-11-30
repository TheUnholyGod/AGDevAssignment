#include "Keyboard.h"
#include <iostream>
#include <sstream>
using namespace std;

#include "KeyboardController.h"
#include "../PlayerInfo/PlayerInfo.h"

const bool _CONTROLLER_KEYBOARD_DEBUG = false;

Keyboard::Keyboard()
{
}


Keyboard::~Keyboard()
{
}

// Create this controller
bool Keyboard::Create(CPlayerInfo* thePlayerInfo)
{
	Controller::Create(thePlayerInfo);
	KeyList[CONTROLLER_MOVEUP] = 'W';
	KeyList[CONTROLLER_MOVEDOWN] = 'S';
	KeyList[CONTROLLER_MOVELEFT] = 'A';
	KeyList[CONTROLLER_MOVERIGHT] = 'D';
	KeyList[CONTROLLER_JUMP] = VK_SPACE;
	KeyList[CONTROLLER_DODGEROLL] = VK_SHIFT;
	KeyList[CONTROLLER_PRIMARYATT] = 'Q';
	KeyList[CONTROLLER_SECONDARYATT] = 'E';
    KeyList[CONTROLLER_INTERACT] = 'F';

	return false;
}


bool Keyboard::Load(std::string _filePath)
{
    return false;
}

// Read from the controller
int Keyboard::Read(const float deltaTime)
{
	Controller::Read(deltaTime);
	if (_CONTROLLER_KEYBOARD_DEBUG)
	return 0;
	int i = 0;
	for (;i < NUM_CONRTOLLER;++i)
	{
		if (KeyboardController::GetInstance()->IsKeyDown(KeyList[i]))
		{
            if (m_inputchecker.find(static_cast<CONTROLLER_STATES>(i)) == m_inputchecker.end())
            {
                if ((i >= CONTROLLER_PRIMARYATT && i <= CONTROLLER_DODGEROLL) && static_cast<CONTROLLER_STATES>(i) != this->m_curr_action)
                {
                    m_input_buffer.push(static_cast<CONTROLLER_STATES>(i));
                    m_inputchecker.insert(static_cast<CONTROLLER_STATES>(i));
                }
                else if (i < CONTROLLER_PRIMARYATT || i > CONTROLLER_DODGEROLL)
                {
                    m_input_buffer.push(static_cast<CONTROLLER_STATES>(i));
                    m_inputchecker.insert(static_cast<CONTROLLER_STATES>(i));
                }
            }
		}
	}
}

void Keyboard::Update(double dt)
{
	Controller::Update(dt);
}
