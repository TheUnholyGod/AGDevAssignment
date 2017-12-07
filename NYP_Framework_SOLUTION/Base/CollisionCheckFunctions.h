#ifndef _COLLISION_H
#define _COLLISION_H

#include "Vector3.h"
#include "EntityBase.h"

class Collision
{
public:
	// Check for overlap
	static bool CheckOverlap(Vector3 thisMinAABB, Vector3 thisMaxAABB, Vector3 thatMinAABB, Vector3 thatMaxAABB);
	// Check if this entity's bounding sphere collided with that entity's bounding sphere 
	static bool CheckSphereCollision(EntityBase *ThisEntity, EntityBase *ThatEntity);
	// Check if this entity collided with another entity, but both must have collider
	static bool CheckAABBCollision(EntityBase *ThisEntity, EntityBase *ThatEntity);
	// Check for intersection between a line segment and a plane
	static bool GetIntersection(const float fDst1, const float fDst2, Vector3 P1, Vector3 P2, Vector3 &Hit);
	// Check for intersection between a line segment and a plane
	static bool CheckLineSegmentPlane(Vector3 line_start, Vector3 line_end,
		Vector3 minAABB, Vector3 maxAABB,
		Vector3 &Hit);
	// Check two positions are within a box region
	static bool InBox(Vector3 Hit, Vector3 B1, Vector3 B2, const int Axis);
};

#endif