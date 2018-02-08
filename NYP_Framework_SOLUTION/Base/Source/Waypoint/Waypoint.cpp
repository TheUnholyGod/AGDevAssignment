#include "Waypoint.h"
#include <limits>
#include <iostream>
using namespace std;

CWaypoint::CWaypoint()
	: position(Vector3(0.0f, 0.0f, 0.0f))
	, m_iID(-1)
{
	relatedWaypoints.clear();
}


CWaypoint::~CWaypoint()
{
	// We clear the relatedWaypoints without deleting 
	// as WaypointManager's destructor will delete the memory
	relatedWaypoints.clear();
}

// Set position
void CWaypoint::SetPosition(Vector3 position)
{
	this->position = position;
}
// Get position
Vector3 CWaypoint::GetPosition(void) const
{
	return position;
}

// Set ID
void CWaypoint::SetID(const int m_iID)
{
	this->m_iID = m_iID;
}
// Get position
int CWaypoint::GetID(void) const
{
	return m_iID;
}

// Add related waypoint
void CWaypoint::AddRelatedWaypoint(CWaypoint* relatedWaypoint)
{
	relatedWaypoints.push_back(relatedWaypoint);
}
// Remove related waypoint
bool CWaypoint::RemoveRelatedWaypoint(const int m_iID)
{
	// If Waypoints has related Waypoints, then we proceed to search.
	if (relatedWaypoints.size() != 0)
	{
		// Iterate through all the Waypoints
		std::vector<CWaypoint*>::iterator it = relatedWaypoints.begin();
		while (it != relatedWaypoints.end())
		{
			if ((*it)->GetID() == m_iID)
			{
				// We erase from the vector, relatedWaypoints, only. DO NOT DELETE!
				it = relatedWaypoints.erase(it);
				return true;
			}
			it++;
		}
	}
	return false;
}

// Get nearest waypoint amongst related Waypoints
CWaypoint* CWaypoint::GetNearestWaypoint(void)
{
	CWaypoint* theNearestWaypoint = NULL;
	float m_fDistance = numeric_limits<float>::max();

	// If Waypoints has related Waypoints, then we proceed to search.
	if (relatedWaypoints.size() != 0)
	{
		// Iterate through all the Waypoints
		std::vector<CWaypoint*>::iterator it = relatedWaypoints.begin();
		while (it != relatedWaypoints.end())
		{
			Vector3 aRelatedWaypoint = (*it)->GetPosition();

			float xDistance = position.x - aRelatedWaypoint.x;
			float zDistance = position.x - aRelatedWaypoint.z;
			float distanceSquared = (float)(xDistance*xDistance + zDistance*zDistance);
			if (m_fDistance > distanceSquared)
			{
				m_fDistance = distanceSquared;
				theNearestWaypoint = (CWaypoint*)(*it);
			}
			it++;
		}
	}
	return theNearestWaypoint;
}

// Get the number of related Waypoints
int CWaypoint::GetNumberOfWaypoints(void) const
{
	return relatedWaypoints.size();
}

// PrintSelf
void CWaypoint::PrintSelf(void)
{
	cout << "Waypoint ID	: " << m_iID << endl;
	cout << " position		: " << position << endl;
	cout << " related waypoints : ";
	// If Waypoints has related Waypoints, then we proceed to search.
	if (relatedWaypoints.size() != 0)
	{
		// Iterate through all the Waypoints
		vector<CWaypoint*>::iterator it = relatedWaypoints.begin();
		while (it != relatedWaypoints.end())
		{
			cout << (*it)->GetID() << endl;
			it++;
		}
	}
	else
		cout << "Nil" << endl;
}
