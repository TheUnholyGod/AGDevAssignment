#pragma once
#include "Strategy.h"
#include "Vector3.h"

class CStrategy_Kill
	: public CStrategy
{
public:
	CStrategy_Kill(void);
	~CStrategy_Kill(void);

	void Update(Vector3& theDestination, Vector3& theEnemyPosition);
	void SetDestination(const float x, const float y);

	int GetDestination_x(void);
	int GetDestination_y(void);
	Vector3 GetDestination(void);

	// The AI states of the enemy
	enum CURRENT_STATE
	{
		IDLE,
		REPEL,
		ATTACK,
		NUM_ENEMY_STATE,
	};
	enum AI_STATE_RANGE
	{
		AI_STATE_REPEL = 25,
		AI_STATE_ATTACK = 125,
		NUM_AI_STATE_RANGE,
	};
	CStrategy_Kill::CURRENT_STATE GetState(void);
	void SetState(CStrategy_Kill::CURRENT_STATE);

private:
	// Enemy AI State
	CStrategy_Kill::CURRENT_STATE CurrentState;
};
