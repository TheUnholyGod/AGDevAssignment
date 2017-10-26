#include "CFurniture.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"


CFurniture::CFurniture(Mesh* mesh, const Vector3& _position,
    const Vector3& _scale) : GenericEntity(mesh)
{
    this->position = _position;
    this->scale = _scale;
    this->maxAABB = this->position + Vector3(5, 10, 5);
    this->minAABB = this->position - Vector3(5, 10, 5);
}

void CFurniture::Render()
{
    MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
    modelStack.PushMatrix();
    modelStack.Translate(position.x, position.y, position.z);
    modelStack.Scale(scale.x, scale.y, scale.z);
    RenderHelper::RenderMesh(this->modelMesh);
    modelStack.PopMatrix();
}