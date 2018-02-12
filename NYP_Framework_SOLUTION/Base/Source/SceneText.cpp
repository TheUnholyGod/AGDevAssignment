#include "SceneText.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>
#include "KeyboardController.h"
#include "MouseController.h"
#include "SceneManager.h"
#include "GraphicsManager.h"
#include "ShaderProgram.h"
#include "EntityManager.h"

#include "GenericEntity.h"
#include "GroundEntity.h"
#include "TextEntity.h"
#include "SpriteEntity.h"
#include "Light.h"
#include "SkyBox/SkyBoxEntity.h"

#include "HardwareAbstraction\Mouse.h"
#include "HardwareAbstraction\Keyboard.h"

#include "Spatial\SpatialPartition.h"
#include "SceneGraph\SceneGraph.h"

#include "Waypoint\WaypointManager.h"
#include "Enemy\Enemy.h" 
#include "QuadTree.h"
#include <iostream>
#include "DebugTool.h"
using namespace std;

SceneText* SceneText::sInstance = new SceneText(SceneManager::GetInstance());

SceneText::SceneText()
{
}

SceneText::SceneText(SceneManager* _sceneMgr)
{
	_sceneMgr->AddScene("Start", this);
}

SceneText::~SceneText()
{
	CSpatialPartition::GetInstance()->RemoveCamera();
	CSceneGraph::GetInstance()->Destroy();
}

void SceneText::Init()
{
	currProg = GraphicsManager::GetInstance()->LoadShader("default", "Shader//Texture.vertexshader", "Shader//Texture.fragmentshader");

	// Tell the shader program to store these uniform locations
	currProg->AddUniform("MVP");
	currProg->AddUniform("MV");
	currProg->AddUniform("MV_inverse_transpose");
	currProg->AddUniform("material.kAmbient");
	currProg->AddUniform("material.kDiffuse");
	currProg->AddUniform("material.kSpecular");
	currProg->AddUniform("material.kShininess");
	currProg->AddUniform("lightEnabled");
	currProg->AddUniform("numLights");
	currProg->AddUniform("lights[0].type");
	currProg->AddUniform("lights[0].position_cameraspace");
	currProg->AddUniform("lights[0].color");
	currProg->AddUniform("lights[0].power");
	currProg->AddUniform("lights[0].kC");
	currProg->AddUniform("lights[0].kL");
	currProg->AddUniform("lights[0].kQ");
	currProg->AddUniform("lights[0].spotDirection");
	currProg->AddUniform("lights[0].cosCutoff");
	currProg->AddUniform("lights[0].cosInner");
	currProg->AddUniform("lights[0].exponent");
	currProg->AddUniform("lights[1].type");
	currProg->AddUniform("lights[1].position_cameraspace");
	currProg->AddUniform("lights[1].color");
	currProg->AddUniform("lights[1].power");
	currProg->AddUniform("lights[1].kC");
	currProg->AddUniform("lights[1].kL");
	currProg->AddUniform("lights[1].kQ");
	currProg->AddUniform("lights[1].spotDirection");
	currProg->AddUniform("lights[1].cosCutoff");
	currProg->AddUniform("lights[1].cosInner");
	currProg->AddUniform("lights[1].exponent");
	currProg->AddUniform("colorTextureEnabled");
	currProg->AddUniform("colorTexture");
	currProg->AddUniform("textEnabled");
	currProg->AddUniform("textColor");

	// Tell the graphics manager to use the shader we just loaded
	GraphicsManager::GetInstance()->SetActiveShader("default");

	lights[0] = new Light();
	GraphicsManager::GetInstance()->AddLight("lights[0]", lights[0]);
	lights[0]->type = Light::LIGHT_DIRECTIONAL;
	lights[0]->position.Set(0, 20, 0);
	lights[0]->color.Set(1, 1, 1);
	lights[0]->power = 1;
	lights[0]->kC = 1.f;
	lights[0]->kL = 0.01f;
	lights[0]->kQ = 0.001f;
	lights[0]->cosCutoff = cos(Math::DegreeToRadian(45));
	lights[0]->cosInner = cos(Math::DegreeToRadian(30));
	lights[0]->exponent = 3.f;
	lights[0]->spotDirection.Set(0.f, 1.f, 0.f);
	lights[0]->name = "lights[0]";

	lights[1] = new Light();
	GraphicsManager::GetInstance()->AddLight("lights[1]", lights[1]);
	lights[1]->type = Light::LIGHT_DIRECTIONAL;
	lights[1]->position.Set(1, 1, 0);
	lights[1]->color.Set(1, 1, 0.5f);
	lights[1]->power = 0.4f;
	lights[1]->name = "lights[1]";

	currProg->UpdateInt("numLights", 1);
	currProg->UpdateInt("textEnabled", 0);

	// Load all the meshes


	MeshBuilder::GetInstance()->GenerateAxes("reference");
	MeshBuilder::GetInstance()->GenerateCrossHair("crosshair");
	MeshBuilder::GetInstance()->GenerateQuad("quad", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("quad")->textureID = LoadTGA("Image//calibri.tga");
	MeshBuilder::GetInstance()->GenerateText("text", 16, 16);
	MeshBuilder::GetInstance()->GetMesh("text")->textureID = LoadTGA("Image//calibri.tga");
	MeshBuilder::GetInstance()->GetMesh("text")->material.kAmbient.Set(1, 0, 0);
	//MeshBuilder::GetInstance()->GenerateOBJ("Chair", "OBJ//chair.obj");
	//	MeshBuilder::GetInstance()->GetMesh("Chair")->textureID = LoadTGA("Image//chair.tga");
	MeshBuilder::GetInstance()->GenerateRing("ring", Color(1, 0, 1), 36, 1, 0.5f);
	MeshBuilder::GetInstance()->GenerateSphere("lightball", Color(1, 1, 1), 18, 36, 1.f);
	MeshBuilder::GetInstance()->GenerateSphere("sphere", Color(1, 0, 0), 18, 36, 1.f);
	MeshBuilder::GetInstance()->GenerateCone("cone", Color(0.5f, 1, 0.3f), 36, 10.f, 10.f);
	MeshBuilder::GetInstance()->GenerateCube("cube", Color(1.0f, 1.0f, 0.0f), 1.0f);
	MeshBuilder::GetInstance()->GetMesh("cone")->material.kDiffuse.Set(0.99f, 0.99f, 0.99f);
	MeshBuilder::GetInstance()->GetMesh("cone")->material.kSpecular.Set(0.f, 0.f, 0.f);
	MeshBuilder::GetInstance()->GenerateQuad("GRASS_DARKGREEN", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("GRASS_DARKGREEN")->textureID = LoadTGA("Image//tiles.tga");
	MeshBuilder::GetInstance()->GenerateQuad("GEO_GRASS_LIGHTGREEN", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("GEO_GRASS_LIGHTGREEN")->textureID = LoadTGA("Image//tiles.tga");

	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_FRONT", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_BACK", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_LEFT", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_RIGHT", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_TOP", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_BOTTOM", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_FRONT")->textureID = LoadTGA("Image//SkyBox//skybox_front.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_BACK")->textureID = LoadTGA("Image//SkyBox//skybox_back.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_LEFT")->textureID = LoadTGA("Image//SkyBox//skybox_left.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_RIGHT")->textureID = LoadTGA("Image//SkyBox//skybox_right.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_TOP")->textureID = LoadTGA("Image//SkyBox//skybox_top.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_BOTTOM")->textureID = LoadTGA("Image//SkyBox//skybox_bottom.tga");
	MeshBuilder::GetInstance()->GenerateCube("cubeSG", Color(1.0f, 0.64f, 0.0f), 1.0f);
	MeshBuilder::GetInstance()->GenerateRay("laser", 10.0f);

	MeshBuilder::GetInstance()->GenerateQuad("GRIDMESH", Color(1, 1, 1), 10.f);




	CSpatialPartition::GetInstance()->Init(100, 100, 10, 10);
	CSpatialPartition::GetInstance()->SetMesh("GRIDMESH");
	CSpatialPartition::GetInstance()->SetCamera(&camera);
	CSpatialPartition::GetInstance()->SetLevelOfDetails(20000.0f, 25000.0f);
	EntityManager::GetInstance()->SetSpatialPartition(CSpatialPartition::GetInstance());
	playerInfo = CPlayerInfo::GetInstance();
	playerInfo->Init();


	// Create and attach the camera to the scene
	//camera.Init(Vector3(0, 0, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));
	camera.Init(playerInfo->GetPos(), playerInfo->GetTarget(), playerInfo->GetUp());
	playerInfo->AttachCamera(&camera);
	GraphicsManager::GetInstance()->AttachCamera(&camera);

	m_kb = new Keyboard();
	m_kb->Create(playerInfo);

	m_mouse = new Mouse();
	m_mouse->Create(playerInfo);

	
	/*for (int i = 0; i < 10; i++)
	{
		float x = 1.0f + (i * rand() % 1000 - 500.0f);
		float y = 1.0f + (i * rand() % 1000 - 500.0f);
		_Enemy->SetRandomSeed(rand());
		_Enemy->Init(x, y);
		_Enemy->SetTerrain(groundEntity);
		_Enemy->SetTarget(_Enemy->GenerateTarget());
		_Enemy = NULL;
	}*/

	m_inputtimer = 0;
	this->ResetScene();
	DebugTool::GetInstance()->BindKey('0', []() { QuadTree::GetInstance()->ToggleRender(); });


	int aWayPoint = CWaypointManager::GetInstance()->AddWaypoint(Vector3(10.0f, 0.0f, 50.0f));
	int anotherWaypoint = CWaypointManager::GetInstance()->AddWaypoint(aWayPoint, Vector3(10.0f, 0.0f, -50.0f));
	CWaypointManager::GetInstance()->AddWaypoint(anotherWaypoint, Vector3(-10.0f, 0.0f, 0.0f));
	CWaypointManager::GetInstance()->PrintSelf();

	//MeshBuilder::GetInstance()->GenerateQuad("INTROSTATE_BKGROUND", Color(1, 1, 1), 1.f);
	//MeshBuilder::GetInstance()->GetMesh("INTROSTATE_BKGROUND")->textureID = LoadTGA("Image//IntroState.tga");
	//float halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2.0f;
	//float halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2.0f;
	//IntroStateBackground = Create::Sprite2DObject("INTROSTATE_BKGROUND",
	//	Vector3(0, 0, 0.0f),
	//	Vector3(800.0f, 600.0f, 0.0f));
}

void SceneText::Update(double dt)
{
	
	/*if (KeyboardController::GetInstance()->IsKeyReleased(VK_SPACE))
	{
		cout << "Loading MenuState" << endl;
		SceneManager::GetInstance()->SetActiveScene("IntroState");
	}*/

	m_inputtimer += dt;

	m_kb->Read(dt);
	m_kb->Update(dt);
	if (m_inputtimer > 0.05f)
	{

		m_inputtimer -= 0.05f;
	}

	// Update our entities
	EntityManager::GetInstance()->Update(dt);
	DebugTool::GetInstance()->Update(dt);
	// THIS WHOLE CHUNK TILL <THERE> CAN REMOVE INTO ENTITIES LOGIC! Or maybe into a scene function to keep the update clean
	if (KeyboardController::GetInstance()->IsKeyDown('1'))
		glEnable(GL_CULL_FACE);
	if (KeyboardController::GetInstance()->IsKeyDown('2'))
		glDisable(GL_CULL_FACE);
	if (KeyboardController::GetInstance()->IsKeyDown('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (KeyboardController::GetInstance()->IsKeyDown('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	if (KeyboardController::GetInstance()->IsKeyDown('5'))
	{
		lights[0]->type = Light::LIGHT_POINT;
	}
	else if (KeyboardController::GetInstance()->IsKeyDown('6'))
	{
		lights[0]->type = Light::LIGHT_DIRECTIONAL;
	}
	else if (KeyboardController::GetInstance()->IsKeyDown('7'))
	{
		lights[0]->type = Light::LIGHT_SPOT;
	}

	//cout << playerInfo->getcurrenthighscore() << endl;

	if (KeyboardController::GetInstance()->IsKeyDown('I'))
		lights[0]->position.z -= (float)(10.f * dt);
	if (KeyboardController::GetInstance()->IsKeyDown('K'))
		lights[0]->position.z += (float)(10.f * dt);
	if (KeyboardController::GetInstance()->IsKeyDown('J'))
		lights[0]->position.x -= (float)(10.f * dt);
	if (KeyboardController::GetInstance()->IsKeyDown('L'))
		lights[0]->position.x += (float)(10.f * dt);
	if (KeyboardController::GetInstance()->IsKeyDown('O'))
		lights[0]->position.y -= (float)(10.f * dt);
	if (KeyboardController::GetInstance()->IsKeyDown('P'))
		lights[0]->position.y += (float)(10.f * dt);


	// if the left mouse button was released
	if (MouseController::GetInstance()->IsButtonReleased(MouseController::LMB))
	{
	}
	if (MouseController::GetInstance()->IsButtonReleased(MouseController::RMB))
	{

	}
	if (MouseController::GetInstance()->IsButtonReleased(MouseController::MMB))
	{
	}
	if (MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_XOFFSET) != 0.0)
	{
	}
	if (MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_YOFFSET) != 0.0)
	{
	}
	if (KeyboardController::GetInstance()->IsKeyPressed('B'))
	{
		this->ResetScene();
	}
	if (KeyboardController::GetInstance()->IsKeyReleased('M'))
	{
		Create::Entity("cube", CPlayerInfo::GetInstance()->GetPos(),Vector3(10,10,10));
		
	}

	if (KeyboardController::GetInstance()->IsKeyReleased('N'))
	{
		QuadTree::GetInstance()->PrintTree();
	}

	// Update the player position and other details based on keyboard and mouse inputs
	playerInfo->Update(dt);

	//camera.Update(dt); // Can put the camera into an entity rather than here (Then we don't have to write this)

	GraphicsManager::GetInstance()->UpdateLights(dt);

	// Update the 2 text object values. NOTE: Can do this in their own class but i'm lazy to do it now :P
	// Eg. FPSRenderEntity or inside RenderUI for LightEntity
	std::ostringstream ss;
	ss.precision(5);
	float fps = (float)(1.f / dt);
	ss << "FPS: " << fps;
	textObj[1]->SetText(ss.str());

	std::ostringstream ss1;
	ss1.precision(4);
	ss1 << "Player:" << playerInfo->GetPos();
	textObj[2]->SetText(ss1.str());
}

void SceneText::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GraphicsManager::GetInstance()->UpdateLightUniforms();

	// Setup 3D pipeline then render 3D
	GraphicsManager::GetInstance()->SetPerspectiveProjection(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	GraphicsManager::GetInstance()->AttachCamera(&camera);
	EntityManager::GetInstance()->Render();

	// Setup 2D pipeline then render 2D
	int halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2;
	int halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2;
	GraphicsManager::GetInstance()->SetOrthographicProjection(-halfWindowWidth, halfWindowWidth, -halfWindowHeight, halfWindowHeight, -10, 10);
	GraphicsManager::GetInstance()->DetachCamera();
	EntityManager::GetInstance()->RenderUI();
}

void SceneText::Exit()
{
	// Detach camera from other entities
	GraphicsManager::GetInstance()->DetachCamera();
	playerInfo->DetachCamera();

	if (playerInfo->DropInstance() == false)
	{
#if _DEBUGMODE==1
		cout << "Unable to drop PlayerInfo class" << endl;
#endif
	}

	// Delete the lights
//	delete lights[0];
	//delete lights[1];
}

void SceneText::ResetScene()
{
	

	QuadTree::GetInstance()->Init(Vector3(1000, 1000, 1000), Vector3());
	EntityManager::GetInstance()->EmptyList();

	

	/*for (int i = 0; i < 50; ++i)
	{
		CEnemy* _Enemy = new CEnemy();
		_Enemy->Init();
		float randomx = 500 - Math::RandFloatMinMax(0, 1000);
		float randomy = 500 - Math::RandFloatMinMax(0, 1000);
		_Enemy->SetPosition(Vector3(randomx, 0, randomy));
	}*/

	_Enemy = new CEnemy();
	_Enemy->Init();
	_Enemy->SetScale(Vector3(5, 5, 5));
	_Enemy->SetPosition(Vector3(20, -10, 20));

	QuadTree::GetInstance()->PrintTree();

	// Create entities into the scene
	Create::Entity("reference", Vector3(0.0f, 0.0f, 0.0f)); // Reference
	Create::Entity("lightball", Vector3(lights[0]->position.x, lights[0]->position.y, lights[0]->position.z)); // Lightball

	GenericEntity* aCube = Create::Entity("clocktower", Vector3(-150.0f, -10.0f, -200.0f));
	aCube->SetCollider(true);
	aCube->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
	aCube->InitLOD("clocktower", "clocktower2", "clocktower3");


	GenericEntity* aCube2 = Create::Entity("clocktower", Vector3(-200.0f, -10.0f, -200.0f));
	aCube2->SetCollider(true);
	aCube2->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
	aCube2->InitLOD("clocktower", "clocktower2", "clocktower3");
	CSceneNode* theNode = CSceneGraph::GetInstance()->AddNode(aCube);
	if (theNode == NULL)
	{
		cout << "EntityManager::Add Entity : Unable to add to scene graph" << endl;
	}
	/*GenericEntity* anotherCube = Create::Entity("cube", Vector3(-20.0f, 1.1f, -20.0f));
	anotherCube->SetCollider(true);
	anotherCube->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));*/
	
	for (int i = 30; i < 50; i++)
	{
		float randomx = 500 - Math::RandFloatMinMax(0, 500);
		float randomy = 500 - Math::RandFloatMinMax(0, 500);
		//House
		GenericEntity* roofentrance = Create::Entity("entrance", Vector3(-randomx, -6.0f, randomy), Vector3(1, 1, 1), true);
		CSceneNode* houseNode1 = CSceneGraph::GetInstance()->AddNode(roofentrance);
		roofentrance->SetCollider(true);
		roofentrance->SetAABB(Vector3(5, 10, 8), Vector3(-5, 0, -5));
		//roofentrance->InitLOD("entrance", "entrance", "entrance");

		GenericEntity* left = Create::Entity("left", Vector3(-randomx, -5.0f, randomy+ 2.0f), Vector3(1, 1, 1), true);
		left->SetCollider(true);
		left->SetAABB(Vector3(10, 10, 51), Vector3(-5, -5, -5));
		CSceneNode* houseNode2 = houseNode1->AddChild(left);
		//houseNode2->ApplyTranslate(0.0f, 0.0f, 0.0f);

		GenericEntity* right = Create::Entity("right", Vector3(-randomx -2.0f, -5.0f, randomy+2.0f), Vector3(1, 1, 1), true);
		right->SetCollider(true);
		right->SetAABB(Vector3(10, 10, 51), Vector3(-5, -5, -5));
		CSceneNode* houseNode3 = houseNode1->AddChild(right);
		//houseNode3->ApplyTranslate(0.0f, 0.0f, 0.0f);

		GenericEntity* floor = Create::Entity("floor", Vector3(-randomx, -5.0f, randomy), Vector3(1, 1, 1), true);
		CSceneNode* houseNode4 = houseNode1->AddChild(floor);
		floor->SetCollider(true);
		floor->SetAABB(Vector3(10, 10, 10), Vector3(-5, -5, -5));
		//houseNode4->ApplyTranslate(0.0f, 0.0f, 0.0f);

		GenericEntity* roof = Create::Entity("roof", Vector3(-randomx, -5.0f, randomy), Vector3(1, 1, 1), true);
		CSceneNode* houseNode5 = houseNode1->AddChild(roof);
		roof->SetCollider(true);
		roof->SetAABB(Vector3(10, 10, 10), Vector3(-5, -5, -5));
		//houseNode5->ApplyTranslate(0.0f, 0.0f, 0.0f);
	}
	//----Tanks

	/*GenericEntity* tankBody = Create::Entity("tankbody", Vector3(0.0f, -6.0f, 0.0f), Vector3(1, 1, 1), true);
	CSceneNode* tankNode1 = CSceneGraph::GetInstance()->AddNode(tankBody);
	tankBody->SetCollider(true);
	tankBody->SetAABB(Vector3(10, 10, 10), Vector3(-5, -5, -5));
	tankBody->InitLOD("tankbody", "tankbody2", "tankbody3");
	GenericEntity* tankTop = Create::Entity("tanktop", Vector3(0.0f, -5.0f, 0.0f),Vector3(1,1,1),true);
	CSceneNode* tankNode2 = tankNode1->AddChild(tankTop);
	tankTop->SetCollider(true);
	tankTop->SetAABB(Vector3(10, 10, 10), Vector3(-5, -5, -5));
	tankNode2->ApplyTranslate(0.0f, 0.0f, 0.0f);
  Rotation
	CUpdateTransformation* rotation = new CUpdateTransformation();
	rotation->ApplyUpdate(1.0f, 0.0f, 1.0f, 0.0f);
	rotation->SetSteps(-120, 60);
	tankNode2->SetUpdateTransformation(rotation);
*/

	//Resize
	

	//End of tanks ------------------------------------

	//Tree------------
	
	for (int i = 0; i < 50; i++)
	{
		float randomx = 500 - Math::RandFloatMinMax(0, 485);
		float randomy = 500 - Math::RandFloatMinMax(0, 485);

		GenericEntity* treeBody = Create::Entity("tree3", Vector3(randomx, -8.0f, randomy), Vector3(5, 5, 5));
		//CSceneNode* treeNode1 = CSceneGraph::GetInstance()->AddNode(treeBody);
		treeBody->SetCollider(true);
		treeBody->SetAABB(Vector3(10, 10, 10), Vector3(-5, -5, -5));
		treeBody->InitLOD("tree2", "tree3", "tree4");


		GenericEntity* treeTop = Create::Entity("tree", Vector3(randomx, -9.0f, randomy), Vector3(5, 5, 5));
		//CSceneNode* treeNode1 = CSceneGraph::GetInstance()->AddNode(treeBody);
		treeTop->SetCollider(true);
		treeTop->SetAABB(Vector3(10, 10, 10), Vector3(-5, -5, -5));
		treeTop->InitLOD("tree", "tree", "tree");
	}

	/*for (int i = 20; i < 50; i++)
	{
		float randomx = 1.0f + (i* rand() % 1000 - 500.0f);
		float randomy = 1.0f + (i* rand() % 1000 - 500.0f);
		GenericEntity* enemy1 = Create::Entity("robot", Vector3(randomx, -9.0f, randomy));
		enemy1->SetScale(Vector3(5, 5, 5));
		enemy1->SetCollider(true);
		enemy1->SetAABB(Vector3(10, 10, 10), Vector3(-5, -5, -5));
		enemy1->InitLOD("robot", "robot2", "robot3");
		
	}*/

	//GenericEntity* treeTop = Create::Entity("tree2", Vector3(20.0f, -8.0f, 0.0f)  ,Vector3(5, 5, 5));
	//CSceneNode* treeNode2 = treeNode1->AddChild(treeTop);
	//treeNode2->ApplyTranslate(0.0f, 0.0f, 0.0f);
	
	//Robot



	//CSceneNode* anotherNode = theNode->AddChild(anotherCube);
	//if (theNode == NULL)
	//{
	//	cout << "EntityManager::Add Entity : Unable to add to scene graph" << endl;
	//}

	groundEntity = Create::Ground("GRASS_DARKGREEN", "GEO_GRASS_LIGHTGREEN");
	//	Create::Text3DObject("text", Vector3(0.0f, 0.0f, 0.0f), "DM2210", Vector3(10.0f, 10.0f, 10.0f), Color(0, 1, 1));
	Create::Sprite2DObject("crosshair", Vector3(0.0f, 0.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f));

	SkyBoxEntity* theSkyBox = Create::SkyBox("SKYBOX_FRONT", "SKYBOX_BACK",
		"SKYBOX_LEFT", "SKYBOX_RIGHT",
		"SKYBOX_TOP", "SKYBOX_BOTTOM");

	// Customise the ground entity
	groundEntity->SetPosition(Vector3(0, -10, 0));
	groundEntity->SetScale(Vector3(100.0f, 100.0f, 100.0f));
	groundEntity->SetGrids(Vector3(10.0f, 1.0f, 10.0f));
	playerInfo->SetTerrain(groundEntity);
	//Enemy->SetTerrain(groundEntity);
	// Setup the 2D entities
	float halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2.0f;
	float halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2.0f;
	float fontSize = 25.0f;
	float halfFontSize = fontSize / 2.0f;
	for (int i = 0; i < 3; ++i)
	{
		textObj[i] = Create::Text2DObject("text", Vector3(-halfWindowWidth, -halfWindowHeight + fontSize*i + halfFontSize, 0.0f), "", Vector3(fontSize, fontSize, fontSize), Color(0.0f, 1.0f, 0.0f));
	}
	textObj[0]->SetText("HEllo world");
	//QuadTree::GetInstance()->PrintTree();

}
