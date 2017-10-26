#include "Strategy.h"
#include <iostream>

using namespace std;

CStrategy::CStrategy()
{
}

CStrategy::~CStrategy()
{
}

void CStrategy::Update(Vector3& theDestination, Vector3& theEnemyPosition)
{
	cout << "void CStrategy::Update()" << endl;
}

int CStrategy::CalculateDistance(Vector3 theDestination, Vector3 theEnemyPosition)
{
	return ((theDestination.x - theEnemyPosition.x)*(theDestination.x - theEnemyPosition.x) + 
			(theDestination.y - theEnemyPosition.y)*(theDestination.y - theEnemyPosition.y));
}
