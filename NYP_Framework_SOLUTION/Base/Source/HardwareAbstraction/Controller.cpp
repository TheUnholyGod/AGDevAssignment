#include "Controller.h"
#include <iostream>
#include <list>
using namespace std;

const bool _CONTROLLER_DEBUG = false;

std::queue<Controller::CONTROLLER_STATES> Controller::m_input_buffer;
std::set<Controller::CONTROLLER_STATES> Controller::m_inputchecker;


Controller::Controller()                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
	: thePlayerInfo(NULL)
{
}


Controller::~Controller()
{
	// We just set thePlayerInfo to NULL without deleting. SceneText will delete this.
	if (thePlayerInfo)
		thePlayerInfo = NULL;
}


// Create this controller
bool Controller::Create(CPlayerInfo* thePlayerInfo)
{
	if (_CONTROLLER_DEBUG)
		cout << "Controller::Create()" << endl;
	this->thePlayerInfo = thePlayerInfo;
	this->controllerfunc[CONTROLLER_MOVEUP] = &Controller::MoveUp;
	this->controllerfunc[CONTROLLER_MOVEDOWN] = &Controller::MoveDown;
	this->controllerfunc[CONTROLLER_MOVELEFT] = &Controller::MoveLeft;
	this->controllerfunc[CONTROLLER_MOVERIGHT] = &Controller::MoveRight;
	this->controllerfunc[CONTROLLER_JUMP] = &Controller::Jump;
	this->controllerfunc[CONTROLLER_DODGEROLL] = &Controller::DodgeRoll;
	this->controllerfunc[CONTROLLER_PRIMARYATT] = &Controller::PrimaryAtt;
	this->controllerfunc[CONTROLLER_SECONDARYATT] = &Controller::SecondaryAtt;
    this->controllerfunc[CONTROLLER_INTERACT] = &Controller::Interact;
	Combo[CONTROLLER_MOVERIGHT][CONTROLLER_PRIMARYATT] = &Controller::PrimaryFrontAtt;
	Combo[CONTROLLER_MOVELEFT][CONTROLLER_PRIMARYATT] = &Controller::PrimaryBackAtt;
	Combo[CONTROLLER_MOVEUP][CONTROLLER_PRIMARYATT] = &Controller::PrimaryUpperAtt;
	Combo[CONTROLLER_MOVEDOWN][CONTROLLER_PRIMARYATT] = &Controller::PrimaryLowerAtt;
	Combo[CONTROLLER_MOVERIGHT][CONTROLLER_SECONDARYATT] = &Controller::SecondaryFrontAtt;
	Combo[CONTROLLER_MOVELEFT][CONTROLLER_SECONDARYATT] = &Controller::SecondaryBackAtt;
	Combo[CONTROLLER_MOVEUP][CONTROLLER_SECONDARYATT] = &Controller::SecondaryUpperAtt;
	Combo[CONTROLLER_MOVEDOWN][CONTROLLER_SECONDARYATT] = &Controller::SecondaryLowerAtt;
	m_curraction_animationtime = 0;
	m_curraction_timer = 0;
	m_curr_action = CONTROLLER_MOVELEFT;
    m_is_free = true;
	return false;
}

// Read from the controller
int Controller::Read(const const float deltaTime)
{
	if (_CONTROLLER_DEBUG)
		cout << "Controller::Read()" << endl;
	return 0;
}

void Controller::Update(double dt)
{
    std::list<CONTROLLER_STATES> prevthree;

	if (m_curraction_animationtime > m_curraction_timer && !m_is_free)
		m_curraction_timer += 0.15f;
	else if ((m_curr_action >= CONTROLLER_JUMP && m_curr_action <= CONTROLLER_DODGEROLL) && m_curraction_animationtime > m_curraction_canceltime && !m_is_free)
		this->m_curr_action = CONTROLLER_MOVELEFT;
	else
	{
		m_is_free = true;
        m_inputchecker.clear();
	}

    CONTROLLER_STATES curr = CONTROLLER_MOVELEFT;

    while (!m_input_buffer.empty())
    {
        CONTROLLER_STATES curr = m_input_buffer.front();
        if (m_is_free)
        {
            m_input_buffer.pop();
            prevthree.push_back(curr);
            if (prevthree.size() > 2)
                prevthree.pop_front();
            m_curr_action = curr;
        }
        else
        {
            bool actionfound = false;
            for (int i = CONTROLLER_JUMP; i < NUM_CONRTOLLER; ++i)
            {
                if (m_inputchecker.find(static_cast<CONTROLLER_STATES>(i)) != m_inputchecker.end() && i != m_curr_action) 
                {
                    if (CancelPrevAction(dt, static_cast<CONTROLLER_STATES>(i)))
                    {
                        curr = static_cast<CONTROLLER_STATES>(i);
                        std::cout << "Action Canceled" << std::endl;
                        while (!m_input_buffer.empty())
                        {
                            curr = m_input_buffer.front();
                            m_input_buffer.pop();
                            prevthree.push_back(curr);
                            if (prevthree.size() > 2)
                                prevthree.pop_front();
                            if (curr == i)
                            {
                                actionfound = true;
								m_curr_action = curr;
                                break;
                            }
                        }
                        this->ClearInputBuffer();
                        break;
                    }
                }
            }
            if(!actionfound)
                break;
        }

        if (curr >= CONTROLLER_JUMP && curr <= CONTROLLER_DODGEROLL)
        {
            ClearInputBuffer();

            if (prevthree.size() > 1)
            {
                CONTROLLER_STATES i = prevthree.front();
                prevthree.pop_front();
                CONTROLLER_STATES j = prevthree.front();
                prevthree.pop_front();
                std::cout << i << ":" << j << std::endl;
				if ((j >= CONTROLLER_PRIMARYATT && j <= CONTROLLER_SECONDARYATT) && (i >= CONTROLLER_MOVELEFT && i <= CONTROLLER_MOVEDOWN))
					(this->*(Combo[i][j]))(dt);
                else 
                    (this->*(controllerfunc[j]))(dt);
            }
            else
                (this->*(controllerfunc[m_curr_action]))(dt);
            break;
        }
        (this->*(controllerfunc[curr]))(dt);

    }
}

bool Controller::CancelPrevAction(double dt, CONTROLLER_STATES next_action)
{
	if (m_curraction_timer > m_curraction_canceltime)
		return false;
	
    return true;
}

bool Controller::MoveUp(double dt)
{
  //  std::cout << "Down" << std::endl;

	this->thePlayerInfo->MoveForward(dt);
	return false;
}

bool Controller::MoveDown(double dt)
{
  //  std::cout << "Up" << std::endl;

	this->thePlayerInfo->MoveBackward(dt);
	return false;
}

bool Controller::MoveLeft(double dt)
{
  //  std::cout << "Left" << std::endl;

	this->thePlayerInfo->MoveLeft(dt);
	return false;
}

bool Controller::MoveRight(double dt)
{
  //  std::cout << "Right" << std::endl;

	this->thePlayerInfo->MoveRight(dt);
	return false;
}


bool Controller::Jump(double dt)
{
	//this->thePlayerInfo->Jump(dt);
  //  std::cout << "Jump" << std::endl;
    m_is_free = false;

	return false;
}

bool Controller::DodgeRoll(double dt)
{
	this->m_curraction_timer = 0;
	this->m_curraction_canceltime = 0.25f;
	this->m_curraction_animationtime = 0.45f;
	//this->thePlayerInfo->DodgeRoll(dt);
    m_is_free = false;
 //   std::cout << "Dodgeroll" << std::endl;
	return false;
}

bool Controller::PrimaryAtt(double dt)
{
	this->m_curraction_timer = 0;
	this->m_curraction_canceltime = 0.25f;
	this->m_curraction_animationtime = 0.5f;
	//this->thePlayerInfo->PrimaryAttack(dt, Melee::COMBO_BASIC1);
    m_is_free = false;

  //  std::cout << "Melee" << std::endl;
	return false;
}

bool Controller::SecondaryAtt(double dt)
{
	this->m_curraction_timer = 0;
	this->m_curraction_canceltime = 0.15f;
	this->m_curraction_animationtime = 0.25f;
	//this->thePlayerInfo->SecondaryAttack(dt);
    m_is_free = false;

 //   std::cout << "Ranged" << std::endl;
	return false;
}

bool Controller::Interact(double dt)
{
    //this->thePlayerInfo->SetInteracting(true);
    return false;
}

bool Controller::PrimaryUpperAtt(double dt)
{
	this->m_curraction_timer = 0;
	this->m_curraction_canceltime = 0.15f;
	this->m_curraction_animationtime = 0.25f;
    m_is_free = false;

	//this->thePlayerInfo->PrimaryAttack(dt, Melee::COMBO_UP1);

//	std::cout << "Upper Melee" << std::endl;
	return false;
}

bool Controller::PrimaryLowerAtt(double dt)
{
	this->m_curraction_timer = 0;
	this->m_curraction_canceltime = 0.15f;
	this->m_curraction_animationtime = 0.25f;
    m_is_free = false;
	//
	//this->thePlayerInfo->PrimaryAttack(dt, Melee::COMBO_DOWN1);

//	std::cout << "Lower Melee" << std::endl;
	return false;
}

bool Controller::PrimaryBackAtt(double dt)
{
	this->m_curraction_timer = 0;
	this->m_curraction_canceltime = 0.15f;
	this->m_curraction_animationtime = 0.25f;
    m_is_free = false;

	//this->thePlayerInfo->PrimaryAttack(dt, Melee::COMBO_BACK1);

//	std::cout << "Back Melee" << std::endl;
	return false;
}

bool Controller::PrimaryFrontAtt(double dt)
{
	this->m_curraction_timer = 0;
	this->m_curraction_canceltime = 0.15f;
	this->m_curraction_animationtime = 0.25f;
    m_is_free = false;

	//this->thePlayerInfo->PrimaryAttack(dt, Melee::COMBO_FRONT1);

//	std::cout << "Front Melee" << std::endl;
	return false;
}

bool Controller::SecondaryUpperAtt(double dt)
{
	this->m_curraction_timer = 0;
	this->m_curraction_canceltime = 0.15f;
	this->m_curraction_animationtime = 0.25f;
    m_is_free = false;
	//this->thePlayerInfo->SecondaryAttack(dt, Ranged::DIR_UP);

	//std::cout << "Upper Ranged" << std::endl;
	return false;
}

bool Controller::SecondaryLowerAtt(double dt)
{
	this->m_curraction_timer = 0;
	this->m_curraction_canceltime = 0.15f;
	this->m_curraction_animationtime = 0.25f;
    m_is_free = false;
//	this->thePlayerInfo->SecondaryAttack(dt, Ranged::DIR_DOWN);

	//std::cout << "Lower Ranged" << std::endl;
	return false;
}

bool Controller::SecondaryBackAtt(double dt)
{
	this->m_curraction_timer = 0;
	this->m_curraction_canceltime = 0.15f;
	this->m_curraction_animationtime = 0.25f;
    m_is_free = false;
//	this->thePlayerInfo->SecondaryAttack(dt, Ranged::DIR_LEFT);

	//std::cout << "Back Ranged" << std::endl;
	return false;
}

bool Controller::SecondaryFrontAtt(double dt)
{
	this->m_curraction_timer = 0;
	this->m_curraction_canceltime = 0.15f;
	this->m_curraction_animationtime = 0.25f;
    m_is_free = false;

	//std::cout << "Front Ranged" << std::endl;
//	this->thePlayerInfo->SecondaryAttack(dt, Ranged::DIR_RIGHT);

	return false;
}

void Controller::ClearInputBuffer()
{
    while (m_input_buffer.size())
        m_input_buffer.pop();
}
