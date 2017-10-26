#include "GoodiesFactory.h"
#include <stdlib.h>
#include <sstream>

// Constructor
CGoodiesFactory::CGoodiesFactory(void)
	: index(0)
{
}

// Destructor
CGoodiesFactory::~CGoodiesFactory(void)
{
}

// Create a goodie and add to the map
CGoodies* CGoodiesFactory::Create(const GoodiesID id, const int tileIndex_Column, const int tileIndex_Row)
{
	CGoodies* theNewGoodies = NULL;

	std::ostringstream _goodieIndex;
	_goodieIndex.precision(3);
	_goodieIndex << tileIndex_Column << "_" << tileIndex_Row;

	switch (id)
	{
	case TREASURECHEST:
		// Create a new CTreasureChest instance
		theNewGoodies = new CTreasureChest;
		// Assign an ID to the goodie
		theNewGoodies->SetID(index++);
		theNewGoodies->SetPos(tileIndex_Column, tileIndex_Row);
		// Add to the map
		AddGoodies(_goodieIndex.str(), theNewGoodies);
		break;
	default:
		break;
	}

	return theNewGoodies;
}

// Get a goodie from this map
CGoodies* CGoodiesFactory::GetGoodies(const std::string& _goodieIndex)
{
	if (goodiesMap.count(_goodieIndex) != 0)
		return goodiesMap[_goodieIndex];

	return nullptr;
}

// Get a goodie from this map using column and row data
CGoodies* CGoodiesFactory::GetGoodies(const int tileIndex_Column, const int tileIndex_Row)
{
	std::ostringstream _goodieIndex;
	_goodieIndex.precision(3);
	_goodieIndex << tileIndex_Column << "_" << tileIndex_Row;

	return GetGoodies(_goodieIndex.str());
}

// Add a goodie to this map
void CGoodiesFactory::AddGoodies(const std::string& _goodieIndex, CGoodies* _newGoodies)
{
	// Trivial Rejection : Invalid pointer provided
	if (_newGoodies == nullptr)
		return;

	// Clean up first if there is an existing goodie with the same name
	RemoveGoodies(_goodieIndex);

	// Add the mesh now
	goodiesMap[_goodieIndex] = _newGoodies;
}

// Remove a goodie from this map
bool CGoodiesFactory::RemoveGoodies(const std::string& _goodieIndex)
{
	CGoodies* currGoodies = GetGoodies(_goodieIndex);
	if (currGoodies != nullptr)
	{
		delete currGoodies;
		goodiesMap.erase(_goodieIndex);
		return true;
	}
	return false;
}

// Remove a goodie from this map
bool CGoodiesFactory::RemoveGoodies(const int tileIndex_Column, const int tileIndex_Row)
{
	std::ostringstream _goodieIndex;
	_goodieIndex.precision(3);
	_goodieIndex << tileIndex_Column << "_" << tileIndex_Row;

	return RemoveGoodies(_goodieIndex.str());
}
