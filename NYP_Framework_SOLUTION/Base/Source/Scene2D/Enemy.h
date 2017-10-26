#pragma once
#include "Vector3.h"
#include "Map.h"
#include "Strategy.h"
#include "Animation.h"

class CEnemy : public CAnimation
{
//	friend class CStrategy;
public:
	CEnemy(void);
	~CEnemy(void);

	// Initialise this class instance
	void Init(CMap* m_cMap = NULL);
	// Set position x of the player
	void SetPos_x(const int pos_x);
	// Set position y of the player
	void SetPos_y( const int pos_y);
	// Set position of the player
	void SetPos( const int pos_x, const int pos_y);
	// Set the destination of this enemy
	void SetDestination(const int pos_x, const int pos_y);
	// Set the destination of this enemy
	void SetDestination(Vector3 pos);

	// Get position x of the player
	int GetPos_x(void) const;
	// Get position y of the player
	int GetPos_y(void) const;
	// Get position of the player in Vector3 form
	Vector3 GetPos(void) const;
	// Get the destination of this enemy
	int GetDestination_x(void) const;
	// Get the destination of this enemy
	int GetDestination_y(void) const;
	// Get the destination of this enemy as a Vector3
	Vector3 GetDestination(void) const;

	// ENEMY Update
	void Update(void);

	// Strategy
	void ChangeStrategy(CStrategy* theNewStrategy, bool bDelete=true);

private:
	// ENEMY's information
	Vector3 theENEMYPosition;

	// The Destination is the position of the Hero
	Vector3 theDestination;

	CStrategy* theStrategy;
};
