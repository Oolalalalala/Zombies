#pragma once

#include "Engine.h"
#include "World.h"

class Game : public Application
{
public:
	Game();
	~Game();
	
	virtual void OnCreate();
	virtual void OnUpdate(float dt);
	virtual void OnDestroy();

private:
	Ref<World> m_world;
};

Application* CreateApplication()
{
	return new Game();
}
