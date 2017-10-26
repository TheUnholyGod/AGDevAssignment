#pragma once

#include "WeaponInfo.h"

class CAssualtRifle :
	public CWeaponInfo
{
public:
	CAssualtRifle();
	virtual ~CAssualtRifle();

	// Initialise this instance to default values
	void Init(void);
};

