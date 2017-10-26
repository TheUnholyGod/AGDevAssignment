#pragma once

#include "Collider\Collider.h"
#include "EntityBase.h"
#include "GenericEntity.h"

#include <string>

class Mesh;

class CFurniture : public GenericEntity
{
public:
    CFurniture(Mesh* mesh, const Vector3& _position,
        const Vector3& _scale = Vector3(1.0f, 1.0f, 1.0f));
    virtual void Render();
};
