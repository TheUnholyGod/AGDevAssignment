#ifndef GENERIC_ENTITY_H
#define GENERIC_ENTITY_H

#include "EntityBase.h"
#include <string>
#include "Collider/Collider.h"
#include "LOD\LevelOfDetails.h"
class Mesh;

class GenericEntity : public EntityBase, public CCollider , public CLevelOfDetails
{
public:
	GenericEntity(Mesh* _modelMesh);
	virtual ~GenericEntity();

	virtual void Update(double _dt);
	virtual void Render();

	// Set the maxAABB and minAABB
	void SetAABB(Vector3 maxAABB, Vector3 minAABB);
	void SetDontRender(bool dontrender) { _dontrender = dontrender; }
	bool GetDontRender() { return _dontrender; }
private:
	Mesh* modelMesh;
	bool _dontrender;
};

namespace Create
{
	GenericEntity* Asset(const std::string& _meshName,
		const Vector3& _position,
		const Vector3& _scale = Vector3(1.0f, 1.0f, 1.0f));

	GenericEntity* Entity(	const std::string& _meshName, 
							const Vector3& _position,
							const Vector3& _scale = Vector3(1.0f, 1.0f, 1.0f), bool dontrender = false);
};

#endif // GENERIC_ENTITY_H