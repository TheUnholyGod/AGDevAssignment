#include "Enemy.h"
#include <iostream>

/********************************************************************************
 Constructor
 ********************************************************************************/
CEnemy::CEnemy(void)
	: theStrategy(NULL)
	, theENEMYPosition(Vector3(0.0f, 0.0f, 0.0f))
{
}

/********************************************************************************
 Destructor
 ********************************************************************************/
CEnemy::~CEnemy(void)
{
	if (theStrategy != NULL)
	{
		delete theStrategy;
		theStrategy = NULL;
	}
}

/********************************************************************************
 Initialise this class instance
 ********************************************************************************/
void CEnemy::Init(CMap* m_cMap)
{
	theENEMYPosition.x=0;
	theENEMYPosition.y=0;
}

/********************************************************************************
 Set position x of the player
 ********************************************************************************/
void CEnemy::SetPos_x(int pos_x)
{
	theENEMYPosition.x = pos_x;
}

/********************************************************************************
 Set position y of the player
 ********************************************************************************/
void CEnemy::SetPos_y(int pos_y)
{
	theENEMYPosition.y = pos_y;
}

/********************************************************************************
 Set position of the player
 ********************************************************************************/
void CEnemy::SetPos(const int pos_x, const int pos_y)
{
	theENEMYPosition.x = pos_x;
	theENEMYPosition.y = pos_y;
}

/********************************************************************************
 Set the destination of this enemy
 ********************************************************************************/
void CEnemy::SetDestination(const int pos_x, const int pos_y)
{
	theDestination.x = pos_x;
	theDestination.y = pos_y;
	if (theStrategy != NULL)
	{
		theStrategy->SetDestination(theDestination.x, theDestination.y);
	}
}

/********************************************************************************
 Set the destination of this enemy
 ********************************************************************************/
void CEnemy::SetDestination(Vector3 pos)
{
	theDestination = pos;
	if (theStrategy != NULL)
	{
		theStrategy->SetDestination(theDestination.x, theDestination.y);
	}
}

/********************************************************************************
 Get position x of the player
 ********************************************************************************/
int CEnemy::GetPos_x(void) const
{
	return theENEMYPosition.x;
}

/********************************************************************************
 Get position y of the player
 ********************************************************************************/
int CEnemy::GetPos_y(void) const
{
	return theENEMYPosition.y;
}

/********************************************************************************
 Get position of the player in Vector3 form
 ********************************************************************************/
Vector3 CEnemy::GetPos(void) const
{
	return theENEMYPosition;
}

/********************************************************************************
 Set the destination of this enemy
 ********************************************************************************/
int CEnemy::GetDestination_x(void) const
{
	return theDestination.x;
}

/********************************************************************************
 Set the destination of this enemy
 ********************************************************************************/
int CEnemy::GetDestination_y(void) const
{
	return theDestination.y;
}

/********************************************************************************
 Get the destination of this enemy as a Vector3
 ********************************************************************************/
Vector3 CEnemy::GetDestination(void) const
{
	return theDestination;
}

/********************************************************************************
 Update
 ********************************************************************************/
void CEnemy::Update(void)
{
	if (theStrategy != NULL)
	{
		theStrategy->Update(theDestination, theENEMYPosition);
	}
}

/********************************************************************************
 Strategy
 ********************************************************************************/
void CEnemy::ChangeStrategy(CStrategy* theNewStrategy, bool bDelete)
{
	if (bDelete == true)
	{
		if (theStrategy != NULL)
		{
			delete theStrategy;
			theStrategy = NULL;
		}
	}

	theStrategy = theNewStrategy;
}
