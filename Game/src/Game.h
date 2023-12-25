#pragma once

#include "Engine.h"
#include <future>

#include "World.h"
#include "Start.h"
#include "Loading.h"
#include "Ending.h"

enum class GameState
{
	None = 0, Menu, Loading, GameCreate, Game, GameDestory, Ending
};

class Game : public Application
{
public:
	Game();
	~Game();
	
	virtual void OnCreate();
	virtual void OnUpdate(float dt);
	virtual void OnDestroy();

	void BeginGame();
	void GoBackMenu();
	void EndGame(int score);

private:
	Ref<World> m_World;
	Ref<Loading> m_Loading;
	Ref<Start> m_Start;
	Ref<Ending> m_Ending;
	GameState m_State;

	std::future<Ref<World>> m_LoadingFuture;
};

Application* CreateApplication()
{
	return new Game();
}
