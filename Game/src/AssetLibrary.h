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

	spongebob_squarepants_patrick, PatrickTexture, PatrickMaterial, 

	white_chinese_dragon, white_chinese_dragonTexture, white_chinese_dragonMaterial,

	snow_dragon, snow_dragonTexture, snow_dragonMaterial,

	shadow_dragon, shadow_dragonTexture, shadow_dragonMaterial,

	monster_skull_hight_poly, monster_skull_hight_polyTexture1, monster_skull_hight_polyMaterial1, monster_skull_hight_polyTexture2, monster_skull_hight_polyMaterial2, monster_skull_hight_polyTexture3, monster_skull_hight_polyMaterial3,

	monster_low_poly, monster_low_polyTexture, monster_low_polyMaterial,

	low_poly_dragon_white, low_poly_dragon_whiteTexture, low_poly_dragon_whiteMaterial,

	fantasy_reverse_horn_dragon_hight_poly, fantasy_reverse_horn_dragon_hight_polyTexture, fantasy_reverse_horn_dragon_hight_polyMaterial,

	dragon_blue_low_poly, dragon_blue_low_polyTexture, dragon_blue_low_polyMaterial,

	baby_yoda_grogu_with_glasses, baby_yoda_grogu_with_glassesTexture1, baby_yoda_grogu_with_glassesMaterial1, baby_yoda_grogu_with_glassesTexture2, baby_yoda_grogu_with_glassesMaterial2, baby_yoda_grogu_with_glassesTexture3, baby_yoda_grogu_with_glassesMaterial3, baby_yoda_grogu_with_glassesTexture4, baby_yoda_grogu_with_glassesMaterial4,


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