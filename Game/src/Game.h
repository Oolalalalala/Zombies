#pragma once

#include "Engine.h"
#include "Patrick.h"
#include "WhiteChineseDragon.h"
#include "SnowDragon.h"
#include "ShadowDragon.h"
#include "MonsterSkull.h"
#include "Monster.h"
#include "FantasyDragon.h"
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

class Game : public Application
{
public:
	Game();
	~Game();
	
	virtual void OnCreate();
	virtual void OnUpdate(float dt);
	virtual void OnDestroy();

private:
	Entity CreateCamera();
	Entity CreateSprite();
	Entity CreateSpriteWithTexture(const std::string& path);
	Entity LoadModel();
	Entity LoadModelWithTexture();
	Entity CreateModelByCopying(Entity copySource);
	Entity CreateText();
	Entity CreateMusic();
	Entity CreateSound();
	Entity CreateFloor(int i, int j);//const std::string& path

private:
	Ref<Scene> m_Scene;
	Entity m_Camera;
	Entity m_Quad, m_QuadWithTexture;
	Entity m_ModelTower, m_ModelGun, m_ModelCopy;
	Entity m_Text;
	Entity m_Music;
	Entity m_Sound;

	Tower* towers[5][4];
	Tower* UpgradeTest;
	Warning* w;
	Entity floor[51][51];

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
	FantasyDragon* fd;
	BlueDragon* bd;
	BabyGrogu* bg;

};

Application* CreateApplication()
{
	return new Game();
}