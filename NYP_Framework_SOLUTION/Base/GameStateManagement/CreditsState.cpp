#include <iostream>
using namespace std;

#include "CreditsState.h"
#include "GL\glew.h"
#include "../Application.h"
#include "LoadTGA.h"
#include "GraphicsManager.h"
#include "MeshBuilder.h"
#include "../TextEntity.h"
#include "RenderHelper.h"
#include "../SpriteEntity.h"
#include "../EntityManager.h"

#include "KeyboardController.h"
#include "SceneManager.h"

CCreditsState::CCreditsState()
{

}
CCreditsState::~CCreditsState()
{

}

void CCreditsState::Init()
{
	// Create and attach the camera to the scene
	camera.Init(Vector3(0, 0, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));
	GraphicsManager::GetInstance()->AttachCamera(&camera);

	// Load all the meshes
	MeshBuilder::GetInstance()->GenerateQuad("CREDITSSTATE_BKGROUND", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("CREDITSSTATE_BKGROUND")->textureID = LoadTGA("Image//CreditState.tga");

	float halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2.0f;
	float halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2.0f;
	CreditsStateBackground = Create::Sprite2DObject("CREDITSSTATE_BKGROUND", 
												 Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Application::GetInstance().resolution[Application::GetInstance().current_res]);
	/*float fontSize = 50.0f;
	float halfFontSize = fontSize / 2.0f;
	textObj[0] = Create::Text2DObject("text", Vector3(halfWindowWidth - 2*fontSize, halfWindowHeight, 0.1f), "TEMP", Vector3(fontSize, fontSize, fontSize), Color(0.0f, 0.0f, 0.0f));
	textObj[0]->SetText("TEMP");
	selection = 0;*/
	elapsedTime = 0.f;
	cout << "CCreditsState loaded\n" << endl;
}
void CCreditsState::Update(double dt)
{
	/*if (KeyboardController::GetInstance()->IsKeyReleased(VK_UP))
	{
		if (selection < 1)
			++selection;
	}
	if (KeyboardController::GetInstance()->IsKeyReleased(VK_DOWN))
	{
		if (selection > 0)
			--selection;
	}
	
	switch (selection)
	{
	case 0:
		textObj[0]->SetText("Resume");
		break;
	case 1:
		textObj[0]->SetText("Menu");
		break;
	}
	if (KeyboardController::GetInstance()->IsKeyReleased(VK_RETURN))
	{
		switch (selection)
		{
		case 0:
			cout << "Loading CGameState" << endl;
			SceneManager::GetInstance()->SetActiveScene("GameState");
			break;
		case 1:
			cout << "Loading CMenuState" << endl;
			SceneManager::GetInstance()->SetActiveScene("MenuState");
			break;
		}
	}*/
	elapsedTime += dt;
	if (elapsedTime > 3.f)
		SceneManager::GetInstance()->SetActiveScene("OptionState");
}
void CCreditsState::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//GraphicsManager::GetInstance()->UpdateLightUniforms();

	// Setup 3D pipeline then render 3D
	GraphicsManager::GetInstance()->SetPerspectiveProjection(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	GraphicsManager::GetInstance()->AttachCamera(&camera);

	// Render the required entities
	EntityManager::GetInstance()->Render();

	// Setup 2D pipeline then render 2D
	GraphicsManager::GetInstance()->SetOrthographicProjection(0, 
															  Application::GetInstance().GetWindowWidth(), 
															  0, 
															  Application::GetInstance().GetWindowHeight(), 
															  -10, 10);
	GraphicsManager::GetInstance()->DetachCamera();

	// Render the required entities
	EntityManager::GetInstance()->RenderUI();
}
void CCreditsState::Exit()
{
	// Remove the entity from EntityManager
	EntityManager::GetInstance()->RemoveEntity(CreditsStateBackground);/*
	EntityManager::GetInstance()->RemoveEntity(textObj[0]);*/
	// Remove the meshes which are specific to CCreditsState
	MeshBuilder::GetInstance()->RemoveMesh("CREDITSSTATE_BKGROUND");

	// Detach camera from other entities
	GraphicsManager::GetInstance()->DetachCamera();
	EntityManager::GetInstance()->ClearEntityList();
}