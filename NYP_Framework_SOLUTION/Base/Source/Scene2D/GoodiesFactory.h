#pragma once

#include "SingletonTemplate.h"
#include "Goodies.h"
#include "TreasureChest.h"
#include <map>

typedef int GoodiesID;
#define TREASURECHEST 100

class CGoodiesFactory : public Singleton<CGoodiesFactory>
{
protected:
	//
	int index;
	// The map of all the goodies created
	std::map<std::string, CGoodies*> goodiesMap;

public:
	// Constructor
	CGoodiesFactory(void);
	// Destructor
	~CGoodiesFactory(void);

	// Create a goodie and add to the map
	CGoodies* Create(const GoodiesID id, const int tileIndex_Column, const int tileIndex_Row);

	// Get a goodie from this map
	CGoodies* GetGoodies(const std::string& _goodieIndex);
	// Get a goodie from this map using column and row data
	CGoodies* GetGoodies(const int tileIndex_Column, const int tileIndex_Row);
	// Add a goodie to this map
	void AddGoodies(const std::string& _goodieIndex, CGoodies* _newGoodies);
	// Remove a goodie from this map
	bool RemoveGoodies(const std::string& _goodieIndex);
	// Remove a goodie from this map
	bool RemoveGoodies(const int tileIndex_Column, const int tileIndex_Row);
	// Get the number of goodies in this map
	int GetNumOfGoodies();
};