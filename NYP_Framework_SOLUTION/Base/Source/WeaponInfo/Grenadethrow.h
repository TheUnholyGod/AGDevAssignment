#pragma once

#include "../WeaponInfo/WeaponInfo.h"

class CGrenadeThrow :
	public CWeaponInfo
{
public:
	CGrenadeThrow();
	virtual ~CGrenadeThrow();

	//Initialise
	void Init(void);

	//Discharge weapon
	void Discharge(Vector3 position, Vector3 target, CPlayerInfo* _source = NULL);
};