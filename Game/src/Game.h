#pragma once

#include "Engine.h"

#include "Patrick.h"
#include "WhiteChineseDragon.h"
#include "SnowDragon.h"
#include "ShadowDragon.h"
#include "MonsterSkull.h"
#include "Monster.h"
#include "BlueDragon.h"
#include "BlueDragon.h"
#include "BabyGrogu.h"




#include "ArcherTower.h"
#include "BallistaTower.h"
#include "CannonTower.h"
#include "PoisonTower.h"
#include "WizardTower.h"

#include "Warning.h"
#include "Map.h"
#include "Start.h"
#include "Gold.h"

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
	Ref<Scene> m_Scene;
	Entity m_Camera;
	Entity m_Quad, m_QuadWithTexture;
	Entity m_ModelTower, m_ModelGun, m_ModelCopy;
	Entity m_Text;
	Entity m_Music;
	Entity m_Sound;

	Map* m_map;

	Tower* towers[5][4];
	Tower* UpgradeTest;
	Warning* w;
	glm::ivec2 PS, NS;
	//Entity floor[51][51];

	//ArcherTower* a;
	//BallistaTower* b;
	//CannonTower* c;
	//PoisonTower* p;
	//WizardTower* w;
	Patrick* p;
	WhiteChineseDragon* wcd; 
	SnowDragon* snd;
	ShadowDragon* shd;
	MonsterSkull* ms;
	Monster* m;
	BlueDragon* bd;
	BabyGrogu* bg;

	Start* s;
	bool Begin = 0;
	Gold* g;
};

Application* CreateApplication()
{
	return new Game();
}
