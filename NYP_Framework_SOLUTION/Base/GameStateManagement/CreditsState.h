#pragma once

#include "Scene.h"
#include "Mtx44.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "../FPSCamera.h"
#include "../SpriteEntity.h"
#include "../TextEntity.h"

class SceneManager;
class CCreditsState : public Scene
{
public:
	CCreditsState();
	~CCreditsState();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:
	FPSCamera camera;
	SpriteEntity* CreditsStateBackground;
	float elapsedTime;
	/*TextEntity* textObj[1];
	int selection;*/
};
