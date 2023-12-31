#pragma once

#include "Engine.h"

//Some model stuff
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
#include "H_PBar.h"
#include "Gold.h"

//Some Controling stuff
#include "Warning.h"
#include "Map.h"


class World
{
public:
	World();
	~World();

	void OnUpdate(float dt);
	void ClearMap();
	void MobSpawn(glm::ivec2, int);
	void MobMove(float);
	void SetEndGameCallback(std::function<void(int)> callback);
	glm::quat df[9];

private:
	Entity CreateCamera();
	Entity CreateSprite();
	Entity CreateSpriteWithTexture(const std::string& path);
	Entity LoadModel();
	Entity LoadModelWithTexture();
	Entity CreateModelByCopying(Entity copySource);
	Entity CreateText();
	Entity CreateMusic();
	Entity CreateSoundEffect(int idx);
	Entity CreateFloor(int i, int j);
	Entity CreateTime();

private:
	Ref<Scene> m_Scene;
	Entity m_Camera;
	Entity m_Quad, m_QuadWithTexture;
	Entity m_ModelTower, m_ModelGun, m_ModelCopy;
	Entity m_Text;
	Entity m_Music;
	Entity m_Sound;
	Entity m_SoundEffect1;
	Entity m_SoundEffect2;
	Entity m_SoundEffect3;
	Entity m_SoundEffect4;
	bool go=0, nervous=0;

	Entity m_Time;

	Map* m_map;

	Tower* towers[51][51];
	Tower* NowBuilding;
	int BuildingIdx = 0;
	Warning* w;
	Warning* BuildWarning;
	glm::ivec2 PS, NS, Building;
	std::vector<glm::ivec2> m_SpawnList;
	std::unordered_map<Enemy*, glm::ivec2> m_MobMap;

	std::vector<glm::ivec2> AttackableTower[51][51];

	glm::quat DefaultDir;
	float PathPeriod = 20.f, LvlUpPeriod = 60.f;

	Entity crystal;
	H_PBar* hpBar;

	Gold* gold;

	float TotalTime = 0.0f;

	std::function<void(int)> m_EndGameCallback;
	bool m_IsCursorMode;
};

