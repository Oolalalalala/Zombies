#pragma once

#include "unordered_map"
#include "Engine.h"

enum class Asset
{
	None = 0,
	TowerTexture, // The texture of all towers
	TowerMaterial, // The material of all towers
	ArcherTowerLevel1Model, ArcherTowerLevel2Model, ArcherTowerLevel3Model, ArcherTowerLevel4Model,
	BallistaTowerLevel1Model, BallistaTowerLevel2Model, BallistaTowerLevel3Model, BallistaTowerLevel4Model,
	CannonTowerLevel1Model, CannonTowerLevel2Model, CannonTowerLevel3Model, CannonTowerLevel4Model,
	PoisonTowerLevel1Model, PoisonTowerLevel2Model, PoisonTowerLevel3Model, PoisonTowerLevel4Model,
	WizardTowerLevel1Model, WizardTowerLevel2Model, WizardTowerLevel3Model, WizardTowerLevel4Model,





	OpenSansFont
};

class AssetLibrary
{
public:
	static void Initialize(Ref<Scene> scene);
	static void ShutDown();

	static AssetHandle Get(Asset asset);
	static Entity GetModel(Asset asset);

private:
	static Ref<Scene> s_Scene;
	static std::unordered_map<Asset, AssetHandle> s_Assets;
	static std::unordered_map<Asset, Entity> s_ModelPrefabs;
};