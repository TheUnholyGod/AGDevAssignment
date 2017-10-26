#include "Scene2D.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "../Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>
#include "KeyboardController.h"
#include "MouseController.h"
#include "SceneManager.h"
#include "GraphicsManager.h"
#include "ShaderProgram.h"
#include "../EntityManager.h"

#include "../GenericEntity.h"
#include "../TextEntity.h"
#include "../SpriteEntity.h"
#include "../Light.h"
#include "Strategy_Kill.h"

#include "GoodiesFactory.h"

#include <iostream>
using namespace std;

CScene2D* CScene2D::sInstance = new CScene2D(SceneManager::GetInstance());

CScene2D::CScene2D()
	: m_cMap(NULL)
	, m_cRearMap(NULL)
	, thePlayerInfo(NULL)
	, m_iNumEnemy(3)
	, Scene2D_Goodies_TreasureChest(NULL)
	, Scene2D_Enemy(NULL)
	, Scene2D_Background(NULL)
	, Scene2D_TileGround(NULL)
	, Scene2D_Hero(NULL)
	, Scene2D_TileTree(NULL)
	, Scene2D_RearStructure(NULL)
	, theEnemy(NULL)
	, Scene2D_Hero_Animated(NULL)
{
}

CScene2D::CScene2D(SceneManager* _sceneMgr)
	: m_cMap(NULL)
	, m_cRearMap(NULL)
	, thePlayerInfo(NULL)
	, m_iNumEnemy(3)
	, Scene2D_Goodies_TreasureChest(NULL)
	, Scene2D_Enemy(NULL)
	, Scene2D_Background(NULL)
	, Scene2D_TileGround(NULL)
	, Scene2D_Hero(NULL)
	, Scene2D_TileTree(NULL)
	, Scene2D_RearStructure(NULL)
	, theEnemy(NULL)
	, Scene2D_Hero_Animated(NULL)
{
	_sceneMgr->AddScene("Scene2D", this);
}

CScene2D::~CScene2D()
{
	// Delete the sprites
	if (Scene2D_Goodies_TreasureChest)
	{
		delete Scene2D_Goodies_TreasureChest;
		Scene2D_Goodies_TreasureChest = NULL;
	}
	if (Scene2D_Enemy)
	{
		delete Scene2D_Enemy;
		Scene2D_Enemy = NULL;
	}
	if (Scene2D_Background)
	{
		delete Scene2D_Background;
		Scene2D_Background = NULL;
	}
	if (Scene2D_TileGround)
	{
		delete Scene2D_TileGround;
		Scene2D_TileGround = NULL;
	}
	if (Scene2D_Hero)
	{
		delete Scene2D_Hero;
		Scene2D_Hero = NULL;
	}
	if (Scene2D_TileTree)
	{
		delete Scene2D_TileTree;
		Scene2D_TileTree = NULL;
	}
	if (Scene2D_RearStructure)
	{
		delete Scene2D_RearStructure;
		Scene2D_RearStructure = NULL;
	}

	if (theEnemy)
	{
		for (int i = 0; i < m_iNumEnemy; ++i)
		{
			delete theEnemy[i];
			theEnemy[i] = NULL;
		}
		delete theEnemy;
		theEnemy = NULL;
	}

	if (Scene2D_Hero_Animated)
	{
		for (int i = 0; i < 7; ++i)
		{
			delete Scene2D_Hero_Animated[i];
			Scene2D_Hero_Animated[i] = NULL;
		}
		delete Scene2D_Hero_Animated;
		Scene2D_Hero_Animated = NULL;
	}

	if (thePlayerInfo->DropInstance() == false)
	{
		cout << "CScene2D: Unable to drop CPlayerInfo2D class" << endl;
	}
	if (m_cRearMap)
	{
		delete m_cRearMap;
		m_cRearMap = NULL;
	}
	if (m_cMap)
	{
		delete m_cMap;
		m_cMap = NULL;
	}
}

void CScene2D::Init()
{
	currProg = GraphicsManager::GetInstance()->LoadShader("default", 
														  "Shader//Texture.vertexshader", 
														  "Shader//Texture.fragmentshader");
	
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

	glDisable(GL_DEPTH_TEST);
	// Enable blend mode
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Load all the meshes
	MeshBuilder::GetInstance()->GenerateAxes("reference");
	MeshBuilder::GetInstance()->GenerateCrossHair("crosshair");
	MeshBuilder::GetInstance()->GenerateQuad("quad", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("quad")->textureID = LoadTGA("Image//calibri.tga");
	MeshBuilder::GetInstance()->GenerateText("text", 16, 16);
	MeshBuilder::GetInstance()->GetMesh("text")->textureID = LoadTGA("Image//calibri.tga");
	MeshBuilder::GetInstance()->GetMesh("text")->material.kAmbient.Set(1, 0, 0);

	// Load background image
	MeshBuilder::GetInstance()->GenerateQuad("SCENE2D_BKGROUND", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("SCENE2D_BKGROUND")->textureID = LoadTGA("Image//Scene2D_Background.tga");

	MeshBuilder::GetInstance()->GenerateQuad("SCENE2D_TILE_GROUND", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("SCENE2D_TILE_GROUND")->textureID = LoadTGA("Image//Scene2D_Tile_Ground.tga");
	MeshBuilder::GetInstance()->GenerateQuad("SCENE2D_TILE_HERO", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("SCENE2D_TILE_HERO")->textureID = LoadTGA("Image//Scene2D_Tile_Hero.tga");
	MeshBuilder::GetInstance()->GenerateQuad("SCENE2D_TILE_TREE", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("SCENE2D_TILE_TREE")->textureID = LoadTGA("Image//Scene2D_Tile_Tree.tga");
	MeshBuilder::GetInstance()->GenerateQuad("SCENE2D_TILE_REARSTRUCTURE", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("SCENE2D_TILE_REARSTRUCTURE")->textureID = 
		LoadTGA("Image//Scene2D_RearStructure.tga");

	MeshBuilder::GetInstance()->GenerateQuad("SCENE2D_TILE_HERO_RIGHT_0", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("SCENE2D_TILE_HERO_RIGHT_0")->textureID = LoadTGA("Image//Scene2D_Tile_Hero_Right_0.tga");
	MeshBuilder::GetInstance()->GenerateQuad("SCENE2D_TILE_HERO_RIGHT_1", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("SCENE2D_TILE_HERO_RIGHT_1")->textureID = LoadTGA("Image//Scene2D_Tile_Hero_Right_1.tga");
	MeshBuilder::GetInstance()->GenerateQuad("SCENE2D_TILE_HERO_RIGHT_2", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("SCENE2D_TILE_HERO_RIGHT_2")->textureID = LoadTGA("Image//Scene2D_Tile_Hero_Right_2.tga");
	MeshBuilder::GetInstance()->GenerateQuad("SCENE2D_TILE_HERO_RIGHT_3", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("SCENE2D_TILE_HERO_RIGHT_3")->textureID = LoadTGA("Image//Scene2D_Tile_Hero_Right_3.tga");
	MeshBuilder::GetInstance()->GenerateQuad("SCENE2D_TILE_HERO_LEFT_1", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("SCENE2D_TILE_HERO_LEFT_1")->textureID = LoadTGA("Image//Scene2D_Tile_Hero_Left_1.tga");
	MeshBuilder::GetInstance()->GenerateQuad("SCENE2D_TILE_HERO_LEFT_2", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("SCENE2D_TILE_HERO_LEFT_2")->textureID = LoadTGA("Image//Scene2D_Tile_Hero_Left_2.tga");
	MeshBuilder::GetInstance()->GenerateQuad("SCENE2D_TILE_HERO_LEFT_3", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("SCENE2D_TILE_HERO_LEFT_3")->textureID = LoadTGA("Image//Scene2D_Tile_Hero_Left_3.tga");

	MeshBuilder::GetInstance()->GenerateQuad("SCENE2D_TILE_ENEMY", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("SCENE2D_TILE_ENEMY")->textureID = LoadTGA("Image//Scene2D_Tile_Enemy.tga");
	MeshBuilder::GetInstance()->GenerateQuad("SCENE2D_TILE_TREASURECHEST", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("SCENE2D_TILE_TREASURECHEST")->textureID = LoadTGA("Image//Scene2D_Tile_TreasureChest.tga");

	// Create entities into the scene
	Create::Entity("reference", Vector3(0.0f, 0.0f, 0.0f)); // Reference
	Create::Sprite2DObject("crosshair", Vector3(0.0f, 0.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f));

	// Initialise and load the tile map
	m_cMap = new CMap();
	m_cMap->Init(800, 600, 24, 32, 600, 1600);
	m_cMap->LoadMap("Image//MapDesign.csv");

	// Create the Goodies
	CreateGoodies();

	m_cRearMap = new CMap();
	m_cRearMap->Init(800, 600, 24, 32, 600, 1600);
	m_cRearMap->LoadMap("Image//RearMapDesign.csv");

	// Create the playerinfo instance, which manages all information about the player
	thePlayerInfo = CPlayerInfo2D::GetInstance();
	thePlayerInfo->Init();
	thePlayerInfo->SetPos(Vector3(50.0f + kiHalfTileWidth, 100.0f + kiHalfTileHeight));
	thePlayerInfo->SetBoundary(Vector3(775.0f, 575.0f, 0.0f), Vector3(25.0f, 25.0f, 0.0f));
	thePlayerInfo->SetTileSize(m_cMap->GetTileSize_Width(), m_cMap->GetTileSize_Height());
	thePlayerInfo->SetMap(m_cMap);
	thePlayerInfo->SetRearMap(m_cRearMap);

	// Setup the 2D entities
	float halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2.0f;
	float halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2.0f;

	// Create the background image
	Scene2D_Background = Create::Sprite2DObject("SCENE2D_BKGROUND",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(800.0f, 600.0f, 0.0f), true);
	Scene2D_TileGround = Create::Sprite2DObject("SCENE2D_TILE_GROUND",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(25.0f, 25.0f, 0.0f));
	Scene2D_Hero = Create::Sprite2DObject("SCENE2D_TILE_HERO",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(25.0f, 25.0f, 0.0f));
	Scene2D_TileTree = Create::Sprite2DObject("SCENE2D_TILE_TREE",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(25.0f, 25.0f, 0.0f));
	Scene2D_RearStructure = Create::Sprite2DObject("SCENE2D_TILE_REARSTRUCTURE",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(25.0f, 25.0f, 0.0f));

	Scene2D_Hero_Animated = new SpriteEntity*[7];
	Scene2D_Hero_Animated[0] = Create::Sprite2DObject("SCENE2D_TILE_HERO_RIGHT_0",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(25.0f, 25.0f, 0.0f));
	Scene2D_Hero_Animated[1] = Create::Sprite2DObject("SCENE2D_TILE_HERO_RIGHT_1",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(25.0f, 25.0f, 0.0f));
	Scene2D_Hero_Animated[2] = Create::Sprite2DObject("SCENE2D_TILE_HERO_RIGHT_2",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(25.0f, 25.0f, 0.0f));
	Scene2D_Hero_Animated[3] = Create::Sprite2DObject("SCENE2D_TILE_HERO_RIGHT_3",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(25.0f, 25.0f, 0.0f));
	Scene2D_Hero_Animated[4] = Create::Sprite2DObject("SCENE2D_TILE_HERO_LEFT_1",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(25.0f, 25.0f, 0.0f));
	Scene2D_Hero_Animated[5] = Create::Sprite2DObject("SCENE2D_TILE_HERO_LEFT_2",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(25.0f, 25.0f, 0.0f));
	Scene2D_Hero_Animated[6] = Create::Sprite2DObject("SCENE2D_TILE_HERO_LEFT_3",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(25.0f, 25.0f, 0.0f));

	Scene2D_Goodies_TreasureChest = Create::Sprite2DObject("SCENE2D_TILE_TREASURECHEST",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(25.0f, 25.0f, 0.0f));
	
	// Set the Animation indices
	thePlayerInfo->SetRightIndices(0, 3);
	thePlayerInfo->SetLeftIndices(4, 6);

	// Create the 3 enemies
	m_iNumEnemy = 3;
	theEnemy = new CEnemy*[m_iNumEnemy];
	for (int i = 0; i < m_iNumEnemy; i++)
	{
		theEnemy[i] = new CEnemy();
		theEnemy[i]->Init(m_cMap);
		theEnemy[i]->ChangeStrategy(new CStrategy_Kill(), false);
	}
	theEnemy[0]->SetPos(450 + 12, 100 + 12);
	theEnemy[1]->SetPos(500 + 12, 100 + 12);
	theEnemy[2]->SetPos(550 + 12, 100 + 12);
	Scene2D_Enemy = Create::Sprite2DObject("SCENE2D_TILE_ENEMY",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(25.0f, 25.0f, 0.0f));

	// Create a text object
	float fontSize = 25.0f;
	float halfFontSize = fontSize / 2.0f;
	for (int i = 0; i < 3; ++i)
	{
		textObj[i] = Create::Text2DObject("text", 
										  Vector3(5, 5 + fontSize*i + halfFontSize, 0.0f), 
										  "", 
										  Vector3(fontSize, fontSize, fontSize), Color(1.0f, 0.0f, 0.0f));
	}
	textObj[0]->SetText("<First Row>");
	textObj[1]->SetText("<Second Row>");
	textObj[2]->SetText("<Third Row>");
}

void CScene2D::Update(double dt)
{
	// Update our entities
	EntityManager::GetInstance()->Update(dt);

	// THIS WHOLE CHUNK TILL <THERE> CAN REMOVE INTO ENTITIES LOGIC! Or maybe into a scene function to keep the update clean
	if(KeyboardController::GetInstance()->IsKeyDown('1'))
		glEnable(GL_CULL_FACE);
	if(KeyboardController::GetInstance()->IsKeyDown('2'))
		glDisable(GL_CULL_FACE);
	if(KeyboardController::GetInstance()->IsKeyDown('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if(KeyboardController::GetInstance()->IsKeyDown('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	// if the left mouse button was released
	if (MouseController::GetInstance()->IsButtonReleased(MouseController::LMB))
	{
		cout << "Left Mouse Button was released!" << endl;
	}
	if (MouseController::GetInstance()->IsButtonReleased(MouseController::RMB))
	{
		cout << "Right Mouse Button was released!" << endl;
	}
	if (MouseController::GetInstance()->IsButtonReleased(MouseController::MMB))
	{
		cout << "Middle Mouse Button was released!" << endl;
	}
	if (MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_XOFFSET) != 0.0)
	{
		cout << "Mouse Wheel has offset in X-axis of " << MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_XOFFSET) << endl;
	}
	if (MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_YOFFSET) != 0.0)
	{
		cout << "Mouse Wheel has offset in Y-axis of " << MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_YOFFSET) << endl;
	}
	// <THERE>

	// Update the thePlayerInfo
	thePlayerInfo->Update(dt);

	// Update the enemies
	for (int i = 0; i < m_iNumEnemy; ++i)
	{
		theEnemy[i]->SetDestination(Vector3(thePlayerInfo->GetPos().x + thePlayerInfo->mapOffset_x, 
											thePlayerInfo->GetPos().y, 
											0));
		theEnemy[i]->Update();
	}

	GraphicsManager::GetInstance()->UpdateLights(dt);
}

void CScene2D::Render()
{
	glClear(GL_COLOR_BUFFER_BIT);// | GL_DEPTH_BUFFER_BIT);

	//GraphicsManager::GetInstance()->UpdateLightUniforms();

	// Setup 3D pipeline then render 3D
	//GraphicsManager::GetInstance()->SetPerspectiveProjection(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	//GraphicsManager::GetInstance()->AttachCamera(&camera);

	// Render the required entities
	//EntityManager::GetInstance()->Render();

	// Setup 2D pipeline then render 2D
	GraphicsManager::GetInstance()->SetOrthographicProjection(0, Application::GetInstance().GetWindowWidth(),
															  0, Application::GetInstance().GetWindowHeight(),
															  -10, 10);
	GraphicsManager::GetInstance()->DetachCamera();

	// Render the required entities
	EntityManager::GetInstance()->RenderUI();

	// Render the rear tile map
	RenderRearTileMap();
	// Render the tile map
	RenderTileMap();
	// Render the Enemy
	RenderEnemy();
	// Render the player
	RenderPlayer();

	textObj[0]->RenderUI();
	textObj[1]->RenderUI();
	textObj[2]->RenderUI();
}

void CScene2D::Exit()
{
	// Detach camera from other entities
	GraphicsManager::GetInstance()->DetachCamera();
}

void CScene2D::RenderTileMap()
{
	int m = 0;
	thePlayerInfo->UpdateMapFineOffset();
	for (int i = 0; i < m_cMap->GetNumOfTiles_Height(); i++)
	{
		for (int k = 0; k < m_cMap->GetNumOfTiles_Width()+1; k++)
		{
			m = thePlayerInfo->GetTileOffset_x() + k;

			// If we have reached the right side of the Map, then do not display the extra column of tiles.
			if (m >= m_cMap->getNumOfTiles_MapWidth())
				break;

			if (m_cMap->theScreenMap[i][m] == 1)
			{
				Scene2D_TileGround->SetPosition(Vector3(k*m_cMap->GetTileSize_Width() + kiHalfTileWidth
															- thePlayerInfo->GetMapFineOffset_x(), 
														575 - i*m_cMap->GetTileSize_Height() + kiHalfTileHeight, 
														0.0f));
				Scene2D_TileGround->RenderUI();
			}
			else if (m_cMap->theScreenMap[i][m] == 2)
			{
				Scene2D_TileTree->SetPosition(Vector3(k*m_cMap->GetTileSize_Width() + kiHalfTileWidth 
															- thePlayerInfo->GetMapFineOffset_x(),
													  575 - i*m_cMap->GetTileSize_Height() + kiHalfTileHeight,
													  0.0f));
				Scene2D_TileTree->RenderUI();
			}
			else if (m_cMap->theScreenMap[i][m] == 10)
			{
				Scene2D_Goodies_TreasureChest->SetPosition(Vector3(k*m_cMap->GetTileSize_Width() + kiHalfTileWidth
					- thePlayerInfo->GetMapFineOffset_x(),
					575 - i*m_cMap->GetTileSize_Height() + kiHalfTileHeight,
					0.0f));
				Scene2D_Goodies_TreasureChest->RenderUI();
			}
		}
	}

	std::ostringstream ss;
	ss.precision(5);
	ss << "CP: " << thePlayerInfo->checkPosition_X << ", " << thePlayerInfo->checkPosition_Y << endl
		<< "P: " << thePlayerInfo->position << endl;
	textObj[1]->SetText(ss.str());

	ss.str("");
	ss.clear();
	ss << "mapOffset_x: " << thePlayerInfo->mapOffset_x << endl;
	textObj[0]->SetText(ss.str());

	ss.str("");
	ss.clear();
	ss << "Enemies: ";
	for (int i = 0; i < m_iNumEnemy; ++i)
	{
		ss << theEnemy[i]->GetPos_x() << "(" << theEnemy[i]->GetPos_x() - thePlayerInfo->mapOffset_x << ")" << ", ";
	}
	textObj[2]->SetText(ss.str());
}

// Render the rear tile map
void CScene2D::RenderRearTileMap(void)
{
	int m = 0;
	thePlayerInfo->UpdateRearMap();
	for (int i = 0; i < m_cRearMap->GetNumOfTiles_Height(); i++)
	{
		for (int k = 0; k < m_cRearMap->GetNumOfTiles_Width() + 1; k++)
		{
			m = thePlayerInfo->GetRearTileOffset_x() + k;

			// If we have reached the right side of the Map, then do not display the extra column of tiles.
			if (m >= m_cRearMap->getNumOfTiles_MapWidth())
				break;

			if (m_cRearMap->theScreenMap[i][m] == 1)
			{
				Scene2D_RearStructure->SetPosition(Vector3(k*m_cRearMap->GetTileSize_Width() + kiHalfTileWidth
					- thePlayerInfo->GetRearMapFineOffset_x(),
					575 - i*m_cRearMap->GetTileSize_Height() + kiHalfTileHeight,
					0.0f));
				Scene2D_RearStructure->RenderUI();
			}
			else if (m_cRearMap->theScreenMap[i][m] == 2)
			{
				Scene2D_RearStructure->SetPosition(Vector3(k*m_cRearMap->GetTileSize_Width() + kiHalfTileWidth
					- thePlayerInfo->GetRearMapFineOffset_x(),
					575 - i*m_cRearMap->GetTileSize_Height() + kiHalfTileHeight,
					0.0f));
				Scene2D_RearStructure->RenderUI();
			}
		}
	}
}

void CScene2D::RenderPlayer()
{
	//	Scene2D_Hero->SetPosition(thePlayerInfo->GetPos());
	//	Scene2D_Hero->RenderUI();

	// Display the player
	Scene2D_Hero_Animated[thePlayerInfo->GetAnimationIndex()]->SetPosition(thePlayerInfo->GetPos());
	Scene2D_Hero_Animated[thePlayerInfo->GetAnimationIndex()]->RenderUI();
}

// Render the enemy
void CScene2D::RenderEnemy(void)
{
	// Render the enemies
	for (int i = 0; i < m_iNumEnemy; ++i)
	{
		int theEnemy_x = theEnemy[i]->GetPos_x() - thePlayerInfo->mapOffset_x;
		int theEnemy_y = theEnemy[i]->GetPos_y();

		if (((theEnemy_x >= thePlayerInfo->GetMinBoundary().x) && (theEnemy_x<thePlayerInfo->GetMaxBoundary().x)) &&
			((theEnemy_y >= thePlayerInfo->GetMinBoundary().y) && (theEnemy_y<thePlayerInfo->GetMaxBoundary().y)))
		{
			Scene2D_Enemy->SetPosition(Vector3(theEnemy_x, theEnemy_y, 0));
			Scene2D_Enemy->RenderUI();
		}
	}
}

// Create the Goodies
void CScene2D::CreateGoodies(void)
{
	for (int i = 0; i < m_cMap->getNumOfTiles_MapHeight(); i++)
	{
		for (int k = 0; k < m_cMap->getNumOfTiles_MapWidth(); k++)
		{
			if (m_cMap->theScreenMap[i][k] == 10)
			{
				CGoodiesFactory::GetInstance()->Create(TREASURECHEST, k, i);
			}
		}
	}
}
