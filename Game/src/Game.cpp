#include "Game.h"
#include "Loading.h"


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

	m_Loading = CreateRef<Loading>();

	m_Ending = CreateRef<Ending>();
	m_Ending->SetRestartCallback(std::bind(&Game::GoBackMenu, this));

	m_State = GameState::Menu;
}

void Game::OnUpdate(float dt)
{
	switch (m_State)
	{
		case GameState::Menu:
		{
			m_Start->OnUpdate(dt);
			break;
		}
		case GameState::Loading:
		{
			m_Loading->OnUpdate(dt);
			m_State = GameState::GameCreate;
			break;
		}
		case GameState::GameCreate:
		{
			m_World = CreateRef<World>();
			m_World->SetEndGameCallback(std::bind(&Game::EndGame, this, std::placeholders::_1));
			m_State = GameState::Game;
			break;
		}
		case GameState::Game:
		{
			m_World->OnUpdate(dt);
			break;
		}
		case GameState::GameDestory:
		{
			m_World.reset();
			m_State = GameState::Ending;
			break;
		}
		case GameState::Ending:
		{
			m_Ending->OnUpdate(dt);
			break;
		}
	}
}

void Game::OnDestroy()
{
}

void Game::BeginGame()
{
	m_State = GameState::Loading;
}

void Game::GoBackMenu()
{
	m_State = GameState::Menu;
}

void Game::EndGame(int score)
{
	m_State = GameState::GameDestory;
	m_Ending->SetScore(score);
}