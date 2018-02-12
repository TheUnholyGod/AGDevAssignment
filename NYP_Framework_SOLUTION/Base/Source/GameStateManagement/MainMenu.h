#pragma once

#include "Scene.h"
#include "Mtx44.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "../FPSCamera.h"
#include "../SpriteEntity.h"
class ShaderProgram;
class SceneManager;

//class TextEntity;
class MenuState : public Scene
{
public:
	MenuState();
	~MenuState();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
	virtual void ResetScene();

private:
	ShaderProgram* currProg;
	FPSCamera camera;
	SpriteEntity* MenuStateBackground;
	//	TextEntity* textObj[3];
};
