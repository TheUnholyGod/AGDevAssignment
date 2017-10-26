#include "AssualtRifle.h"


CAssualtRifle::CAssualtRifle()
{
}


CAssualtRifle::~CAssualtRifle()
{
}

// Initialise this instance to default values
void CAssualtRifle::Init(void)
{
	// Call the parent's Init method
	CWeaponInfo::Init();

	// The number of ammunition in a magazine for this weapon
	magRounds = 30;
	// The maximum number of ammunition for this magazine for this weapon
	maxMagRounds = 30;
	// The current total number of rounds currently carried by this player
	totalRounds = 120;
	// The max total number of rounds currently carried by this player
	maxTotalRounds = 120;

	// The time between shots
	timeBetweenShots = 0.1111;
	// The elapsed time (between shots)
	elapsedTime = 0.0;
	// Boolean flag to indicate if weapon can fire now
	bFire = true;
}
