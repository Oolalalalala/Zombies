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
	m_world = CreateRef<World>();
}

void Game::OnUpdate(float dt)
{
	m_world->OnUpdate(dt);
}

void Game::OnDestroy()
{
}
