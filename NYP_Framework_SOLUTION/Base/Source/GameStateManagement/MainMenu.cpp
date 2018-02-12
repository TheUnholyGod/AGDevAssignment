#include <iostream>
using namespace std;

#include "MainMenu.h"
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
#include "../QuadTree.h"
#include "../GenericEntity.h"
MenuState::MenuState()
{

}
MenuState::~MenuState()
{

}

void MenuState::Init()
{
	currProg = GraphicsManager::GetInstance()->LoadShader("default", "Shader//Texture.vertexshader", "Shader//Texture.fragmentshader");
	// Create and attach the camera to the scene
	GraphicsManager::GetInstance()->SetActiveShader("default");
	camera.Init(Vector3(0, 0, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));
	GraphicsManager::GetInstance()->AttachCamera(&camera);

	// Load all the meshes
	MeshBuilder::GetInstance()->GenerateQuad("MenuState_BKGROUND", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("MenuState_BKGROUND")->textureID = LoadTGA("Image//MainMenu.tga");
	float halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2.0f;
	float halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2.0f;
	MenuStateBackground = Create::Sprite2DObject("MenuState_BKGROUND", 
													Vector3(halfWindowWidth, halfWindowHeight, 0.0f), 
													Vector3(800.0f, 600.0f, 0.0f));

	cout << "MenuState loaded\n" << endl;
	//QuadTree::GetInstance()->Init(Vector3(1000, 1000, 1000), Vector3());
	//EntityManager::GetInstance()->EmptyList();
	//QuadTree::GetInstance()->PrintTree();
	//EntityManager::GetInstance()->RenderUI();
	//GenericEntity* aCube = Create::Entity("clocktower", Vector3(-150.0f, -10.0f, -200.0f));
	//aCube->SetCollider(true);
	//aCube->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
	//aCube->InitLOD("clocktower", "clocktower2", "clocktower3");
}
void MenuState::Update(double dt)
{
	if (KeyboardController::GetInstance()->IsKeyPressed(VK_SPACE))
	{
		cout << "Loading MenuState" << endl;
		SceneManager::GetInstance()->SetActiveScene("GameState");
	}

	if (KeyboardController::GetInstance()->IsKeyReleased('1'))
	{
		cout << "Loading MenuState" << endl;
		SceneManager::GetInstance()->SetActiveScene("Options");
	}
}
void MenuState::Render()
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
void MenuState::Exit()
{
	// Remove the entity from EntityManager
	EntityManager::GetInstance()->RemoveEntity(MenuStateBackground);

	// Remove the meshes which are specific to MenuState
	MeshBuilder::GetInstance()->RemoveMesh("MenuState_BKGROUND");

	// Detach camera from other entities
	GraphicsManager::GetInstance()->DetachCamera();
}

void MenuState::ResetScene()
{

}