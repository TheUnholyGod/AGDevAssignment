#pragma once

#include "Scene.h"
#include "Mtx44.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "../FPSCamera.h"
#include "../SpriteEntity.h"
#include "../TextEntity.h"

class SceneManager;
class COptionState : public Scene
{
public:
	COptionState();
	~COptionState();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:
	enum States {
		STATE_OPTION,
		STATE_CREDITS
	}myState;
	FPSCamera camera;
	SpriteEntity* OptionStateBackground;
	SpriteEntity* CreditsStateBackground;
	/*SpriteEntity* currButton;
	SpriteEntity* resumeButton;
	SpriteEntity* menuButton;*/
	TextEntity* textObj[1];
	int selection;
	float elapsedTime;
	Vector3 myRes;
	//	TextEntity* textObj[3];
};
