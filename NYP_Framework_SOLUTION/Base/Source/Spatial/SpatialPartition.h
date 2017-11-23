#pragma once

#include "Vector3.h"
#include "Grid.h"
#include "EntityBase.h"

class CSpatialPartition
{
protected:
	static CSpatialPartition *sp_instance;
	// Constructor
	CSpatialPartition(void);

	// Variables
	CGrid* theGrid;
	int xSize;
	int zSize;
	int xGridSize;
	int zGridSize;
	int xNumOfGrid;
	int zNumOfGrid;
	float yOffset;
	std::string _meshName; // Name of the mesh

public:
	static CSpatialPartition *GetInstance()
	{
		if (!sp_instance)
			sp_instance = new CSpatialPartition;
		return sp_instance;
	}
	static bool DropInstance()
	{
		if (sp_instance)
		{
			delete sp_instance;
			sp_instance = NULL;
			return true;
		}
		return false;
	}
	~CSpatialPartition(void);

	// Initialise the spatial partition
	bool Init(	const int xGridSize, const int zGridSize, 
				const int xNumOfGrid, const int zNumOfGrid, 
				const float yOffset = -9.9f);

	// Set a particular grid's Mesh
	void SetMesh(const std::string& _meshName);

	// ApplyMesh
	void ApplyMesh(void);

	// Update the spatial partition
	void Update(void);
	// Render the spatial partition
	void Render(Vector3* theCameraPosition = NULL);

	// Get xSize of the entire spatial partition
	int GetxSize(void) const;
	// Get zSize of the entire spatial partition
	int GetzSize(void) const;
	// Get xSize
	int GetxGridSize(void) const;
	// Get zNumOfGrid
	int GetzGridSize(void) const;
	// Get xNumOfGrid
	int GetxNumOfGrid(void) const;
	// Get zNumOfGrid
	int GetzNumOfGrid(void) const;

	// Get a particular grid
	CGrid GetGrid(const int xIndex, const int zIndex) const;

	// Get vector of objects from this Spatial Partition
	vector<EntityBase*> GetObjects(Vector3 position, const float radius);

	// Add a new object
	void Add(EntityBase* theObject);
	// Remove but not delete object from this grid
	void Remove(EntityBase* theObject);

	// Calculate the squared distance from camera to a grid's centrepoint
	float CalculateDistanceSquare(Vector3* theCameraPosition, const int xIndex, const int zIndex);

	//PrintSelf
	void PrintSelf() const;

	// The vector of objects due for migration to another grid
	vector<EntityBase*> MigrationList;
};
