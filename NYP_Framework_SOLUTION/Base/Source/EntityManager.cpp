#include "EntityManager.h"
#include "EntityBase.h"
#include "Collider/Collider.h"

#include <iostream>
using namespace std;

// Update all entities
void EntityManager::Update(double _dt)
{
	// Update all entities
	std::list<EntityBase*>::iterator it, end;
	end = entityList.end();
	for (it = entityList.begin(); it != end; ++it)
	{
		(*it)->Update(_dt);
	}

	// Check for Collision amongst entities with collider properties
	CheckForCollision();

	// Clean up entities that are done
	it = entityList.begin();
	while (it != end)
	{
		if ((*it)->IsDone())
		{
			// Delete if done
			delete *it;
			it = entityList.erase(it);
		}
		else
		{
			// Move on otherwise
			++it;
		}
	}
}

// Render all entities
void EntityManager::Render()
{
	// Render all entities
	std::list<EntityBase*>::iterator it, end;
	end = entityList.end();
	for (it = entityList.begin(); it != end; ++it)
	{
		(*it)->Render();
	}
}

// Render the UI entities
void EntityManager::RenderUI()
{
	// Render all entities UI
	std::list<EntityBase*>::iterator it, end;
	end = entityList.end();
	for (it = entityList.begin(); it != end; ++it)
	{
		(*it)->RenderUI();
	}
}

// Add an entity to this EntityManager
void EntityManager::AddEntity(EntityBase* _newEntity)
{
	entityList.push_back(_newEntity);
}

// Remove an entity from this EntityManager
bool EntityManager::RemoveEntity(EntityBase* _existingEntity)
{
	// Find the entity's iterator
	std::list<EntityBase*>::iterator findIter = std::find(entityList.begin(), entityList.end(), _existingEntity);

	// Delete the entity if found
	if (findIter != entityList.end())
	{
		delete *findIter;
		findIter = entityList.erase(findIter);
		return true;
	}
	// Return false if not found
	return false;
}

// Constructor
EntityManager::EntityManager()
{
}

// Destructor
EntityManager::~EntityManager()
{
}

// Check for overlap
bool EntityManager::CheckOverlap(Vector3 thisMinAABB, Vector3 thisMaxAABB, Vector3 thatMinAABB, Vector3 thatMaxAABB)
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
bool EntityManager::CheckSphereCollision(EntityBase *ThisEntity, EntityBase *ThatEntity)
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
bool EntityManager::CheckAABBCollision(EntityBase *ThisEntity, EntityBase *ThatEntity)
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

// Check if any Collider is colliding with another Collider
bool EntityManager::CheckForCollision(void)
{
	// Check for Collision
	std::list<EntityBase*>::iterator colliderThis, colliderThisEnd;
	std::list<EntityBase*>::iterator colliderThat, colliderThatEnd;

	colliderThisEnd = entityList.end();
	for (colliderThis = entityList.begin(); colliderThis != colliderThisEnd; ++colliderThis)
	{
		if ((*colliderThis)->HasCollider())
		{
			// This object was derived from a CCollider class, then it will have Collision Detection methods
			//CCollider *thisCollider = dynamic_cast<CCollider*>(*colliderThis);
			EntityBase *thisEntity = dynamic_cast<EntityBase*>(*colliderThis);

			// Check for collision with another collider class
			colliderThatEnd = entityList.end();
			int counter = 0;
			for (colliderThat = colliderThis; colliderThat != colliderThatEnd; ++colliderThat)
			{
				if (colliderThat == colliderThis)
					continue;

				if ((*colliderThat)->HasCollider())
				{
					// This object was derived from a CCollider class, then it will have Collision Detection methods
					EntityBase *thatEntity = dynamic_cast<EntityBase*>(*colliderThat);
					if (CheckSphereCollision(thisEntity, thatEntity) == true)
					{
						if (CheckAABBCollision(thisEntity, thatEntity) == true)
						{
							thisEntity->SetIsDone(true);
							thatEntity->SetIsDone(true);
						}
					}
				}
			}
		}
	}
	return false;
}
