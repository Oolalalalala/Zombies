#pragma once

#include "Engine.h"
#include "World.h"
#include "Start.h"


class Game : public Application
{
public:
	Game();
	~Game();
	
	virtual void OnCreate();
	virtual void OnUpdate(float dt);
	virtual void OnDestroy();

	void BeginGame();
private:
	Ref<World> m_world;
	Ref<Start> m_Start;
	bool m_GameStarted = false;
};

Application* CreateApplication()
{
	return new Game();
}
