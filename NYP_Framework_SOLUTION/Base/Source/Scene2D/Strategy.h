#pragma once
#include "Vector3.h"

class CStrategy
{
public:
	CStrategy(void);
	~CStrategy(void);

	virtual void Update(Vector3& theDestination, Vector3& theEnemyPosition) = 0;

	virtual void SetDestination(const float x, const float y) = 0;

	virtual int GetDestination_x(void) = 0;
	virtual int GetDestination_y(void) = 0;
	virtual Vector3 GetDestination(void) = 0;

protected:
	int CalculateDistance(Vector3 theDestination, Vector3 theEnemyPosition);

	// The Destination for this Strategy
	Vector3 theDestination;
};
