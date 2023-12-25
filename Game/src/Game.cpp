#include "Game.h"

Game::Game()
	:Application()
{

}

Game::~Game()
{
}

void Game::OnCreate()
{
	m_Start = CreateRef<Start>();
	m_Start->SetStartCallback(std::bind(&Game::BeginGame, this));
	m_world = CreateRef<World>();
}

void Game::OnUpdate(float dt)
{
	if (m_GameStarted)
		m_world->OnUpdate(dt);
	else
		m_Start->OnUpdate(dt);
}

void Game::OnDestroy()
{
}

void Game::BeginGame()
{
	m_GameStarted = true;
}