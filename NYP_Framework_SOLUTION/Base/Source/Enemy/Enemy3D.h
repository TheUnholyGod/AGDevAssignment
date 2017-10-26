#pragma once
#include "../GenericEntity.h"
#include "../GroundEntity.h"

#include <vector>
using namespace std;

class Mesh;

class CEnemy3D :
	public GenericEntity
{
protected:
	Mesh* modelMesh;

	Vector3 defaultPosition, defaultTarget, defaultUp;
	Vector3 target, up;
	Vector3 maxBoundary, minBoundary;
	GroundEntity* m_pTerrain;
	
	double m_dSpeed;
	double m_dAcceleration;
	float m_fElapsedTimeBeforeUpdate;

public:
	CEnemy3D(Mesh* _modelMesh);
	virtual ~CEnemy3D();

	void Init(void);
	// Reset this player instance to default
	void Reset(void);

	// Set position
	void SetPos(const Vector3& pos);
	// Set target
	void SetTarget(const Vector3& target);
	// Set Up
	void SetUp(const Vector3& up);
	// Set the boundary for the player info
	void SetBoundary(Vector3 max, Vector3 min);
	// Set the terrain for the player info
	void SetTerrain(GroundEntity* m_pTerrain);

	// Get position
	Vector3 GetPos(void) const;
	// Get target
	Vector3 GetTarget(void) const;
	// Get Up
	Vector3 GetUp(void) const;
	// Get the terrain for the player info
	GroundEntity* GetTerrain(void);

	// Update
	void Update(double dt = 0.0333f);

	// Constrain the position within the borders
	void Constrain(void);
	// Render
	void Render(void);
};

namespace Create
{
	CEnemy3D* Enemy3D(const std::string& _meshName,
						const Vector3& _position,
						const Vector3& _scale = Vector3(1.0f, 1.0f, 1.0f));
};
