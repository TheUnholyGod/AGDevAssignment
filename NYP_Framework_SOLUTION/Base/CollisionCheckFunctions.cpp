#include "CollisionCheckFunctions.h"
#include "Collider\Collider.h"

bool Collision::CheckOverlap(Vector3 thisMinAABB, Vector3 thisMaxAABB, Vector3 thatMinAABB, Vector3 thatMaxAABB)
{
	// Check if this object is overlapping that object
	/*
	if (((thatMinAABB.x >= thisMinAABB.x) && (thatMinAABB.x <= thisMaxAABB.x) &&
	(thatMinAABB.y >= thisMinAABB.y) && (thatMinAABB.y <= thisMaxAABB.y) &&
	(thatMinAABB.z >= thisMinAABB.z) && (thatMinAABB.z <= thisMaxAABB.z))
	||
	((thatMaxAABB.x >= thisMinAABB.x) && (thatMaxAABB.x <= thisMaxAABB.x) &&
	(thatMaxAABB.y >= thisMinAABB.y) && (thatMaxAABB.y <= thisMaxAABB.y) &&
	(thatMaxAABB.z >= thisMinAABB.z) && (thatMaxAABB.z <= thisMaxAABB.z)))
	*/
	if (((thatMinAABB >= thisMinAABB) && (thatMinAABB <= thisMaxAABB))
		||
		((thatMaxAABB >= thisMinAABB) && (thatMaxAABB <= thisMaxAABB)))
	{
		return true;
	}

	// Check if that object is overlapping this object
	/*
	if (((thisMinAABB.x >= thatMinAABB.x) && (thisMinAABB.x <= thatMaxAABB.x) &&
	(thisMinAABB.y >= thatMinAABB.y) && (thisMinAABB.y <= thatMaxAABB.y) &&
	(thisMinAABB.z >= thatMinAABB.z) && (thisMinAABB.z <= thatMaxAABB.z))
	||
	((thisMaxAABB.x >= thatMinAABB.x) && (thisMaxAABB.x <= thatMaxAABB.x) &&
	(thisMaxAABB.y >= thatMinAABB.y) && (thisMaxAABB.y <= thatMaxAABB.y) &&
	(thisMaxAABB.z >= thatMinAABB.z) && (thisMaxAABB.z <= thatMaxAABB.z)))
	*/
	if (((thisMinAABB >= thatMinAABB) && (thisMinAABB <= thatMaxAABB))
		||
		((thisMaxAABB >= thatMinAABB) && (thisMaxAABB <= thatMaxAABB)))
	{
		return true;
	}

	// Check if this object is within that object
	/*
	if (((thisMinAABB.x >= thatMinAABB.x) && (thisMaxAABB.x <= thatMaxAABB.x) &&
	(thisMinAABB.y >= thatMinAABB.y) && (thisMaxAABB.y <= thatMaxAABB.y) &&
	(thisMinAABB.z >= thatMinAABB.z) && (thisMaxAABB.z <= thatMaxAABB.z))
	&&
	((thisMaxAABB.x >= thatMinAABB.x) && (thisMaxAABB.x <= thatMaxAABB.x) &&
	(thisMaxAABB.y >= thatMinAABB.y) && (thisMaxAABB.y <= thatMaxAABB.y) &&
	(thisMaxAABB.z >= thatMinAABB.z) && (thisMaxAABB.z <= thatMaxAABB.z)))
	*/
	if (((thisMinAABB >= thatMinAABB) && (thisMaxAABB <= thatMaxAABB))
		&&
		((thisMaxAABB >= thatMinAABB) && (thisMaxAABB <= thatMaxAABB)))
		return true;

	// Check if that object is within this object
	/*
	if (((thatMinAABB.x >= thisMinAABB.x) && (thatMinAABB.x <= thisMaxAABB.x) &&
	(thatMinAABB.y >= thisMinAABB.y) && (thatMinAABB.y <= thisMaxAABB.y) &&
	(thatMinAABB.z >= thisMinAABB.z) && (thatMinAABB.z <= thisMaxAABB.z))
	&&
	((thatMaxAABB.x >= thisMinAABB.x) && (thatMaxAABB.x <= thisMaxAABB.x) &&
	(thatMaxAABB.y >= thisMinAABB.y) && (thatMaxAABB.y <= thisMaxAABB.y) &&
	(thatMaxAABB.z >= thisMinAABB.z) && (thatMaxAABB.z <= thisMaxAABB.z)))
	*/
	if (((thatMinAABB >= thisMinAABB) && (thatMinAABB <= thisMaxAABB))
		&&
		((thatMaxAABB >= thisMinAABB) && (thatMaxAABB <= thisMaxAABB)))
		return true;

	return false;
}

// Check if this entity's bounding sphere collided with that entity's bounding sphere 
bool Collision::CheckSphereCollision(EntityBase *ThisEntity, EntityBase *ThatEntity)
{
	// Get the colliders for the 2 entities
	CCollider *thisCollider = dynamic_cast<CCollider*>(ThisEntity);
	CCollider *thatCollider = dynamic_cast<CCollider*>(ThatEntity);

	// Get the minAABB and maxAABB for each entity
	Vector3 thisMinAABB = ThisEntity->GetPosition() + thisCollider->GetMinAABB();
	Vector3 thisMaxAABB = ThisEntity->GetPosition() + thisCollider->GetMaxAABB();
	Vector3 thatMinAABB = ThatEntity->GetPosition() + thatCollider->GetMinAABB();
	Vector3 thatMaxAABB = ThatEntity->GetPosition() + thatCollider->GetMaxAABB();

	// if Radius of bounding sphere of ThisEntity plus Radius of bounding sphere of ThatEntity is 
	// greater than the distance squared between the 2 reference points of the 2 entities,
	// then it could mean that they are colliding with each other.
	if (DistanceSquaredBetween(thisMinAABB, thisMaxAABB) + DistanceSquaredBetween(thatMinAABB, thatMaxAABB) >
		DistanceSquaredBetween(ThisEntity->GetPosition(), ThatEntity->GetPosition()) * 2.0)
	{
		return true;
	}

	return false;
}

// Check if this entity collided with another entity, but both must have collider
bool Collision::CheckAABBCollision(EntityBase *ThisEntity, EntityBase *ThatEntity)
{
	// Get the colliders for the 2 entities
	CCollider *thisCollider = dynamic_cast<CCollider*>(ThisEntity);
	CCollider *thatCollider = dynamic_cast<CCollider*>(ThatEntity);

	// Get the minAABB and maxAABB for each entity
	Vector3 thisMinAABB = ThisEntity->GetPosition() + thisCollider->GetMinAABB();
	Vector3 thisMaxAABB = ThisEntity->GetPosition() + thisCollider->GetMaxAABB();
	Vector3 thatMinAABB = ThatEntity->GetPosition() + thatCollider->GetMinAABB();
	Vector3 thatMaxAABB = ThatEntity->GetPosition() + thatCollider->GetMaxAABB();

	// Check for overlap
	if (CheckOverlap(thisMinAABB, thisMaxAABB, thatMinAABB, thatMaxAABB))
		return true;

	// if AABB collision check fails, then we need to check the other corners of the bounding boxes to 
	// Do more collision checks with other points on each bounding box
	Vector3 altThisMinAABB = Vector3(thisMinAABB.x, thisMinAABB.y, thisMaxAABB.z);
	Vector3 altThisMaxAABB = Vector3(thisMaxAABB.x, thisMaxAABB.y, thisMinAABB.z);
	//	Vector3 altThatMinAABB = Vector3(thatMinAABB.x, thatMinAABB.y, thatMaxAABB.z);
	//	Vector3 altThatMaxAABB = Vector3(thatMaxAABB.x, thatMaxAABB.y, thatMinAABB.z);

	// Check for overlap
	if (CheckOverlap(altThisMinAABB, altThisMaxAABB, thatMinAABB, thatMaxAABB))
		return true;

	return false;
}

// Check where a line segment between two positions intersects a plane
bool Collision::GetIntersection(const float fDst1, const float fDst2, Vector3 P1, Vector3 P2, Vector3 &Hit)
{
	if ((fDst1 * fDst2) >= 0.0f)
		return false;
	if (fDst1 == fDst2)
		return false;
	Hit = P1 + (P2 - P1) * (-fDst1 / (fDst2 - fDst1));
	return true;
}

// Check two positions are within a box region
bool Collision::InBox(Vector3 Hit, Vector3 B1, Vector3 B2, const int Axis)
{
	if (Axis == 1 && Hit.z > B1.z && Hit.z < B2.z && Hit.y > B1.y && Hit.y < B2.y) return true;
	if (Axis == 2 && Hit.z > B1.z && Hit.z < B2.z && Hit.x > B1.x && Hit.x < B2.x) return true;
	if (Axis == 3 && Hit.x > B1.x && Hit.x < B2.x && Hit.y > B1.y && Hit.y < B2.y) return true;
	return false;
}

// Check for intersection between a line segment and a plane
bool Collision::CheckLineSegmentPlane(Vector3 line_start, Vector3 line_end,
	Vector3 minAABB, Vector3 maxAABB,
	Vector3 &Hit)
{
	if ((GetIntersection(line_start.x - minAABB.x, line_end.x - minAABB.x, line_start, line_end, Hit) &&
		InBox(Hit, minAABB, maxAABB, 1))
		|| (GetIntersection(line_start.y - minAABB.y, line_end.y - minAABB.y, line_start, line_end, Hit) &&
			InBox(Hit, minAABB, maxAABB, 2))
		|| (GetIntersection(line_start.z - minAABB.z, line_end.z - minAABB.z, line_start, line_end, Hit) &&
			InBox(Hit, minAABB, maxAABB, 3))
		|| (GetIntersection(line_start.x - maxAABB.x, line_end.x - maxAABB.x, line_start, line_end, Hit) &&
			InBox(Hit, minAABB, maxAABB, 1))
		|| (GetIntersection(line_start.y - maxAABB.y, line_end.y - maxAABB.y, line_start, line_end, Hit) &&
			InBox(Hit, minAABB, maxAABB, 2))
		|| (GetIntersection(line_start.z - maxAABB.z, line_end.z - maxAABB.z, line_start, line_end, Hit) &&
			InBox(Hit, minAABB, maxAABB, 3)))
		return true;

	return false;
}