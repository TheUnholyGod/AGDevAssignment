#include "WaypointManager.h"
#include <iostream>
using namespace std;

// Allocating and initializing CPlayerInfo's static data member.  
// The pointer is allocated but not the object's constructor.
CWaypointManager *CWaypointManager::s_instance = 0;

CWaypointManager::CWaypointManager()
	: m_iID(-1)
{
	listOfWaypoints.clear();
}


CWaypointManager::~CWaypointManager()
{
	// If listOfWaypoints is not empty, then proceed to delete
	if (listOfWaypoints.size() != 0)
	{
		// Iterate through all the Waypoints
		// Destroy the children first
		vector<CWaypoint*>::iterator it;
		for (it = listOfWaypoints.begin(); it != listOfWaypoints.end(); ++it)
		{
			delete *it;
		}
	}
	// We delete the waypoints here
	listOfWaypoints.clear();
}

// Add waypoint
int CWaypointManager::AddWaypoint(Vector3 position)
{
	m_iID++;
	CWaypoint* aNewWaypoint = new CWaypoint();
	aNewWaypoint->SetID(m_iID);
	aNewWaypoint->SetPosition(position);
	listOfWaypoints.push_back(aNewWaypoint);
	return m_iID;
}

// Add waypoint based on its m_iID
int CWaypointManager::AddWaypoint(const int m_iID, Vector3 position)
{
	// If Waypoints is not empty, then we proceed to search.
	if (listOfWaypoints.size() != 0)
	{
		for (unsigned i = 0; i < listOfWaypoints.size(); ++i)
		{
			CWaypoint* aWayPoint = listOfWaypoints[i];

			if (aWayPoint->GetID() == m_iID)
			{
				(this->m_iID)++;
				CWaypoint* aNewWaypoint = new CWaypoint();
				aNewWaypoint->SetID(this->m_iID);
				aNewWaypoint->SetPosition(position);
				listOfWaypoints.push_back(aNewWaypoint);

				// Add the current Waypoint as a related waypoint to the new Waypoint
				aNewWaypoint->AddRelatedWaypoint(aWayPoint);
				// Add this new Waypoint as a related waypoint to the current Waypoint
				aWayPoint->AddRelatedWaypoint(aNewWaypoint);

				return 	this->m_iID;
			}
		}
	}
	return -1;
}

// Remove waypoint based on its m_iID
bool CWaypointManager::RemoveWaypoint(const int m_iID)
{
	bool result = false;

	// If Waypoints is not empty, then we proceed to search.
	if (listOfWaypoints.size() != 0)
	{
		// Iterate through all the Waypoints
		std::vector<CWaypoint*>::iterator it = listOfWaypoints.begin();
		while (it != listOfWaypoints.end())
		{
			if ((*it)->GetID() == m_iID)
			{
				// Remove all references to this Waypoint from other Waypoints
				RemoveRelatedWaypoint(m_iID);

				// Remove this Waypoint from the WaypointManager
				delete *it;
				it = listOfWaypoints.erase(it);
				result = true;
				break;
			}
			if (it != listOfWaypoints.end())
				it++;
		}
	}
	return result;
}
// Get a Waypoint based on its ID
CWaypoint* CWaypointManager::GetWaypoint(const int m_iID)
{
	// If Waypoints is not empty, then we proceed to search.
	if (listOfWaypoints.size() != 0)
	{
		// Iterate through all the Waypoints
		std::vector<CWaypoint*>::iterator it = listOfWaypoints.begin();
		while (it != listOfWaypoints.end())
		{
			if ((*it)->GetID() == m_iID)
			{
				return (CWaypoint*)(*it);
			}
			it++;
		}
	}
	return NULL;
}

// Get nearest waypoint amongst related Waypoints
CWaypoint* CWaypointManager::GetNearestWaypoint(Vector3 aPosition)
{
	CWaypoint* theNearestWaypoint = NULL;
	float m_fDistance = numeric_limits<float>::max();

	// If Waypoints has related Waypoints, then we proceed to search.
	if (listOfWaypoints.size() != 0)
	{
		// Iterate through all the Waypoints
		std::vector<CWaypoint*>::iterator it = listOfWaypoints.begin();
		while (it != listOfWaypoints.end())
		{
			Vector3 aRelatedWaypoint = (*it)->GetPosition();

			float xDistance = aPosition.x - aRelatedWaypoint.x;
			float zDistance = aPosition.z - aRelatedWaypoint.z;
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
int CWaypointManager::GetNumberOfWaypoints(void) const
{
	return listOfWaypoints.size();
}

// PrintSelf
void CWaypointManager::PrintSelf(void)
{
	cout << "================================================================================" << endl;
	cout << "CWaypointManager::PrintSelf" << endl;
	// If Waypoints has related Waypoints, then we proceed to search.
	if (listOfWaypoints.size() != 0)
	{
		// Iterate through all the Waypoints
		vector<CWaypoint*>::iterator it = listOfWaypoints.begin();
		while (it != listOfWaypoints.end())
		{
			(*it)->PrintSelf();
			it++;
		}
	}
	cout << "================================================================================" << endl;
}

// Remove related waypoint
void CWaypointManager::RemoveRelatedWaypoint(const int m_iID)
{
	// Iterate through all the Waypoints
	std::vector<CWaypoint*>::iterator it = listOfWaypoints.begin();
	while (it != listOfWaypoints.end())
	{
		(*it)->RemoveRelatedWaypoint(m_iID);
		it++;
	}
}
