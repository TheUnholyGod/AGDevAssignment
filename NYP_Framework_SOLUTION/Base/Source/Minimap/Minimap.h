#pragma once

#include "EntityBase.h"
#include "SingletonTemplate.h"
#include "Mesh.h"

class CMinimap : public EntityBase, public Singleton<CMinimap>
{
public:
	enum SPRITE_RENDERMODE
	{
		MODE_2D,
		MODE_3D,
		NUM_MODE
	};

	CMinimap(void);
	virtual ~CMinimap(void);

	Mesh* m_cMinimap_Background;
	Mesh* m_cMinimap_Border;
	Mesh* m_cMinimap_Avatar;
	Mesh* m_cMinimap_Target;
	Mesh* m_cMinimap_Stencil;

	// Initialise this class instance
	bool Init(void);

	// Set the target (a.k.a. the enemies in the minimap)
	bool SetTarget(Mesh* aTarget);
	// Get the target (a.k.a. the enemies in the minimap)
	Mesh* GetTarget(void) const;

	// Set the background mesh to this class
	bool SetBackground(Mesh* aBackground);
	// Get the background mesh to this class
	Mesh* GetBackground(void) const;

	// Set the Border mesh to this class
	bool SetBorder(Mesh* aBorder);
	// Get the Border mesh to this class
	Mesh* GetBorder(void) const;

	// Set the Avatar mesh to this class
	bool SetAvatar(Mesh* anAvatar);
	// Get the Avatar mesh to this class
	Mesh* GetAvatar(void) const;

	// Set rotation angle of avatar
	bool SetAngle(const int angle);
	// Get rotation angle of avatar
	int GetAngle(void) const;

	// Set the Stencil mesh to this class
	bool SetStencil(Mesh* aStencil);
	// Get the Stencil mesh to this class
	Mesh* GetStencil(void) const;

	//// Set position of avatar in minimap
	//bool SetPosition(const int x, const int y);
	//// Get position x of avatar in minimap
	//int GetPosition_x(void) const;
	//// Get position y of avatar in minimap
	//int GetPosition_y(void) const;

	//// Set size of minimap (for calculation of avatar in minimap)
	//bool SetSize(const int size_x, const int size_y);
	//// Get size of minimap (for calculation of avatar in minimap)
	//int GetSize_x(void) const;
	//// Get size of minimap (for calculation of avatar in minimap)
	//int GetSize_y(void) const;

	virtual void RenderUI();

protected:
	// Rotation from First Angle
	int m_iAngle;
	//// Offset in the minimap
	//int m_iPos_x, m_iPos_y;
	//// Minimap size
	//int m_iSize_x, m_iSize_y;

	SPRITE_RENDERMODE mode;
};

namespace Create
{
	CMinimap* Minimap(const bool m_bAddToLibrary = true);
};
