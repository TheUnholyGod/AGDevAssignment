#pragma once
#include "Waypoint.h"
#include "Vector3.h"
#include <vector>
using namespace std;

class CWaypointManager
{
protected:
	static CWaypointManager *s_instance;
	CWaypointManager(void);

public:
	static CWaypointManager *GetInstance()
	{
		if (!s_instance)
			s_instance = new CWaypointManager;
		return s_instance;
	}
	static bool DropInstance()
	{
		if (s_instance)
		{
			delete s_instance;
			s_instance = NULL;
			return true;
		}
		return false;
	}
public:
	virtual ~CWaypointManager();

	// Add waypoint
	int AddWaypoint(Vector3 position);
	// Add waypoint based on its m_iID
	int AddWaypoint(const int m_iID, Vector3 position);
	// Remove waypoint based on its m_iID
	bool RemoveWaypoint(const int m_iID);

	// Get a Waypoint based on its ID
	CWaypoint* GetWaypoint(const int m_iID);
	// Get nearest waypoint amongst related Waypoints
	CWaypoint* GetNearestWaypoint(Vector3 aPosition);
	// Get the number of related Waypoints
	int GetNumberOfWaypoints(void) const;

	// PrintSelf
	void PrintSelf(void);

protected:
	// The next ID to assign to a Waypoint
	int m_iID;

	vector<CWaypoint*> listOfWaypoints;

	// Remove related waypoint
	void RemoveRelatedWaypoint(const int m_iID);
};