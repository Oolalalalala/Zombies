#include "Game.h"

#include <iostream>

int BlockStatus[51][51] = { 0 };

Game::Game()
	: Application()
{
	
}

Game::~Game()
{
}

void Game::OnCreate()
{
	
	// Create scene
	m_Scene = CreateRef<Scene>("Demo");
	AssetLibrary::Initialize(m_Scene);

	s=new Start();
	//g=new Gold(s->s_Scene);
	
	// Default lookat block
	PS = glm::ivec2(-1, -1);
	NS = PS;


	//create floor 51*51
	m_map = new Map(m_Scene, 51);

	



	//update MapStatus
	for (int i = 1;i < 6;i++) {
		for (int j = 1;j < 5;j++) {
			m_map->ChangeColor(m_Scene, i, j, 1);//tower
		}
	}



	s->SetStartCallback(std::bind(&Game::BeginGame, this));

}

float speed = 70.0f;
void Game::OnUpdate(float dt) // dt現在是正確的了!
{
	if (Begin==0)
		s->OnUpdate(dt);
	//if (IO::IsKeyDown(KeyCode::L)) g->UpdateGold(10);
}

void Game::OnDestroy()
{
	AssetLibrary::ShutDown();
}

void Game::BeginGame()
{
	Begin = 1;
	delete s;
}
