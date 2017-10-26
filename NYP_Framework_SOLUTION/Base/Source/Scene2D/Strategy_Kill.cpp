#include "Strategy_Kill.h"
#include <iostream>

using namespace std;

/********************************************************************************
Constructor
********************************************************************************/
CStrategy_Kill::CStrategy_Kill()
{
}

/********************************************************************************
Destructor
********************************************************************************/
CStrategy_Kill::~CStrategy_Kill()
{
}

/********************************************************************************
 Update method
********************************************************************************/
void CStrategy_Kill::Update(Vector3& theDestination, Vector3& theEnemyPosition)
{
	// Decide which state to change to
	int distanceHeroToEnemy = CalculateDistance(theDestination, theEnemyPosition);
	if (distanceHeroToEnemy < AI_STATE_ATTACK*AI_STATE_ATTACK)
	{		
		if (distanceHeroToEnemy < AI_STATE_REPEL*AI_STATE_REPEL)
			CurrentState = REPEL;
		else
			CurrentState = ATTACK;
	}
	else
		CurrentState = IDLE;

	// Based on the current state, move the enemy
	switch(CurrentState)
	{
	case ATTACK:
		theEnemyPosition.x = theEnemyPosition.x + (theDestination.x - theEnemyPosition.x > 0 ? 2 : -2);
		break;
	case REPEL:
		theEnemyPosition.x = theEnemyPosition.x + (theDestination.x - theEnemyPosition.x > 0 ? -2 : 2 );
		break;
	default:
		// Do nothing if idling
		break;
	}
}

/********************************************************************************
 Set the destination for this strategy
 ********************************************************************************/
void CStrategy_Kill::SetDestination(const float x, const float y)
{
	theDestination.x = x;
	theDestination.y = y;
}

/********************************************************************************
 Get the destination for this strategy
 ********************************************************************************/
int CStrategy_Kill::GetDestination_x(void)
{
	return theDestination.x;
}

/********************************************************************************
 Get the destination for this strategy
 ********************************************************************************/
int CStrategy_Kill::GetDestination_y(void)
{
	return theDestination.y;
}

/********************************************************************************
 Get the destination for this strategy
 ********************************************************************************/
Vector3 CStrategy_Kill::GetDestination(void)
{
	return theDestination;
}

/********************************************************************************
 Get the FSM state for this strategy
 ********************************************************************************/
CStrategy_Kill::CURRENT_STATE CStrategy_Kill::GetState(void)
{
	return CurrentState;
}

/********************************************************************************
 Set the FSM state for this strategy
 ********************************************************************************/
void CStrategy_Kill::SetState(CStrategy_Kill::CURRENT_STATE theEnemyState)
{
	CurrentState = theEnemyState;
}
