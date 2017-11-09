#pragma once

#include "../PlayerInfo/PlayerInfo.h"
#include <queue>
#include <set>

class Controller
{
protected:
	CPlayerInfo* thePlayerInfo;

public:

	//Add more based on how many functions you have
    enum CONTROLLER_STATES
    {
		CONTROLLER_MOVELEFT,
		CONTROLLER_MOVERIGHT,
        CONTROLLER_MOVEUP,
        CONTROLLER_MOVEDOWN,
		CONTROLLER_JUMP,
		CONTROLLER_PRIMARYATT,
		CONTROLLER_SECONDARYATT,
		CONTROLLER_DODGEROLL,
        CONTROLLER_INTERACT,
        NUM_CONRTOLLER,
    };

	Controller();
	virtual ~Controller();

	// Create this controller
	virtual bool Create(CPlayerInfo* thePlayerInfo = NULL);
	// Read from the controller
	virtual int Read(const float deltaTime);

	virtual void Update(double dt);

	virtual bool CancelPrevAction(double dt, CONTROLLER_STATES next_action);

	bool(Controller::*controllerfunc[NUM_CONRTOLLER])(double dt);

	virtual bool MoveUp(double dt);
	virtual bool MoveDown(double dt);
	virtual bool MoveLeft(double dt);
	virtual bool MoveRight(double dt);
	virtual bool Jump(double dt);
	virtual bool DodgeRoll(double dt);
	virtual bool PrimaryAtt(double dt);
	virtual bool SecondaryAtt(double dt);
    virtual bool Interact(double dt);

	virtual bool PrimaryUpperAtt(double dt);
	virtual bool PrimaryLowerAtt(double dt);
	virtual bool PrimaryBackAtt(double dt);
	virtual bool PrimaryFrontAtt(double dt);

	virtual bool SecondaryUpperAtt(double dt);
	virtual bool SecondaryLowerAtt(double dt);
	virtual bool SecondaryBackAtt(double dt);
	virtual bool SecondaryFrontAtt(double dt);

    void ClearInputBuffer();
protected:
	static std::queue<CONTROLLER_STATES> m_input_buffer;
	CONTROLLER_STATES m_curr_action;
	double m_curraction_animationtime, m_curraction_timer, m_curraction_canceltime;
	bool(Controller::*Combo[NUM_CONRTOLLER][NUM_CONRTOLLER])(double dt);
	static std::set<CONTROLLER_STATES> m_inputchecker;
    bool m_is_free;
};

