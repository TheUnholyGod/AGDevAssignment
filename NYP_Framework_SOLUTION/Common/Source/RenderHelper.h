#ifndef RENDER_HELPER_H
#define RENDER_HELPER_H

#include "Vertex.h"

class Mesh;

class RenderHelper
{
public:
	static void RenderMesh(Mesh* _mesh);
	static void RenderMeshWithLight(Mesh* _mesh);
	static void RenderText(Mesh* _mesh, const std::string& _text, Color _color);
    static void DrawLine(Vector3 origin, Vector3 destination);
};

#endif // RENDER_HELPER_H