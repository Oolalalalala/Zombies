#include "AssetLibrary.h"


Ref<Scene> AssetLibrary::s_Scene;
std::unordered_map<Asset, AssetHandle> AssetLibrary::s_Assets;
std::unordered_map<Asset, Entity> AssetLibrary::s_ModelPrefabs;


void AssetLibrary::Initialize(Ref<Scene> scene)
{
	AssetManager::SetAssetDirectory("asset");

	

	

	s_Assets[Asset::TowerTexture] = AssetManager::LoadTexture("Model/towers/texture/Texture_MAp_fortress_elves.png");
	s_Assets[Asset::TowerMaterial] = AssetManager::CreateMaterial(s_Assets[Asset::TowerTexture]);

	s_ModelPrefabs[Asset::ArcherTowerLevel1Model] = Model::Load(scene, "Model/towers/fbx/Full/_archer_tower_LVL_1.fbx", { s_Assets[Asset::TowerMaterial] });
	s_ModelPrefabs[Asset::ArcherTowerLevel2Model] = Model::Load(scene, "Model/towers/fbx/Full/_archer_tower_LVL_2.fbx", { s_Assets[Asset::TowerMaterial] });
	s_ModelPrefabs[Asset::ArcherTowerLevel3Model] = Model::Load(scene, "Model/towers/fbx/Full/_archer_tower_LVL_3.fbx", { s_Assets[Asset::TowerMaterial] });
	s_ModelPrefabs[Asset::ArcherTowerLevel4Model] = Model::Load(scene, "Model/towers/fbx/Full/_archer_tower_LVL_4.fbx", { s_Assets[Asset::TowerMaterial] });

	s_ModelPrefabs[Asset::BallistaTowerLevel1Model] = Model::Load(scene, "Model/towers/fbx/Full/_Ballista_tower_LVL_1.fbx", { s_Assets[Asset::TowerMaterial] });
	s_ModelPrefabs[Asset::BallistaTowerLevel2Model] = Model::Load(scene, "Model/towers/fbx/Full/_Ballista_tower_LVL_2.fbx", { s_Assets[Asset::TowerMaterial] });
	s_ModelPrefabs[Asset::BallistaTowerLevel3Model] = Model::Load(scene, "Model/towers/fbx/Full/_Ballista_tower_LVL_3.fbx", { s_Assets[Asset::TowerMaterial] });
	s_ModelPrefabs[Asset::BallistaTowerLevel4Model] = Model::Load(scene, "Model/towers/fbx/Full/_Ballista_tower_LVL_4.fbx", { s_Assets[Asset::TowerMaterial] });

	s_ModelPrefabs[Asset::CannonTowerLevel1Model] = Model::Load(scene, "Model/towers/fbx/Full/_Cannon_tower_LVL_1.fbx", { s_Assets[Asset::TowerMaterial] });
	s_ModelPrefabs[Asset::CannonTowerLevel2Model] = Model::Load(scene, "Model/towers/fbx/Full/_Cannon_tower_LVL_2.fbx", { s_Assets[Asset::TowerMaterial] });
	s_ModelPrefabs[Asset::CannonTowerLevel3Model] = Model::Load(scene, "Model/towers/fbx/Full/_Cannon_tower_LVL_3.fbx", { s_Assets[Asset::TowerMaterial] });
	s_ModelPrefabs[Asset::CannonTowerLevel4Model] = Model::Load(scene, "Model/towers/fbx/Full/_Cannon_tower_LVL_4.fbx", { s_Assets[Asset::TowerMaterial] });

	s_ModelPrefabs[Asset::PoisonTowerLevel1Model] = Model::Load(scene, "Model/towers/fbx/Full/_Poison_tower_LVL_1.fbx", { s_Assets[Asset::TowerMaterial] });
	s_ModelPrefabs[Asset::PoisonTowerLevel2Model] = Model::Load(scene, "Model/towers/fbx/Full/_Poison_tower_LVL_2.fbx", { s_Assets[Asset::TowerMaterial] });
	s_ModelPrefabs[Asset::PoisonTowerLevel3Model] = Model::Load(scene, "Model/towers/fbx/Full/_Poison_tower_LVL_3.fbx", { s_Assets[Asset::TowerMaterial] });
	s_ModelPrefabs[Asset::PoisonTowerLevel4Model] = Model::Load(scene, "Model/towers/fbx/Full/_Poison_tower_LVL_4.fbx", { s_Assets[Asset::TowerMaterial] });

	s_ModelPrefabs[Asset::WizardTowerLevel1Model] = Model::Load(scene, "Model/towers/fbx/Full/_Wizard_tower_LVL_1.fbx", { s_Assets[Asset::TowerMaterial] });
	s_ModelPrefabs[Asset::WizardTowerLevel2Model] = Model::Load(scene, "Model/towers/fbx/Full/_Wizard_tower_LVL_2.fbx", { s_Assets[Asset::TowerMaterial] });
	s_ModelPrefabs[Asset::WizardTowerLevel3Model] = Model::Load(scene, "Model/towers/fbx/Full/_Wizard_tower_LVL_3.fbx", { s_Assets[Asset::TowerMaterial] });
	s_ModelPrefabs[Asset::WizardTowerLevel4Model] = Model::Load(scene, "Model/towers/fbx/Full/_Wizard_tower_LVL_4.fbx", { s_Assets[Asset::TowerMaterial] });

	s_Assets[Asset::PatrickTexture] = AssetManager::LoadTexture("Model/enemy/spongebob_squarepants_patrick/textures/MI_Patrick_PBR_baseColor.png");
	s_Assets[Asset::PatrickMaterial] = AssetManager::CreateMaterial(s_Assets[Asset::PatrickTexture]);
	s_ModelPrefabs[Asset::spongebob_squarepants_patrick] = Model::Load(scene, "Model/enemy/spongebob_squarepants_patrick/scene.gltf", { s_Assets[Asset::PatrickMaterial] });
	s_ModelPrefabs[Asset::spongebob_squarepants_patrick].GetComponent<MeshRendererComponent>().Enabled = false;

	s_Assets[Asset::white_chinese_dragonTexture] = AssetManager::LoadTexture("Model/enemy/white_chinese_dragon/textures/BaiLongZuoQi_baseColor.png");
	s_Assets[Asset::white_chinese_dragonMaterial] = AssetManager::CreateMaterial(s_Assets[Asset::white_chinese_dragonTexture]);
	s_ModelPrefabs[Asset::white_chinese_dragon] = Model::LoadSkinned(scene, "Model/enemy/white_chinese_dragon/scene.gltf", { s_Assets[Asset::white_chinese_dragonMaterial] });
	s_ModelPrefabs[Asset::white_chinese_dragon].GetComponent<SkinnedMeshRendererComponent>().Enabled = false;

	s_Assets[Asset::snow_dragonTexture] = AssetManager::LoadTexture("Model/enemy/snow_dragon/textures/zq127_binglong_1_baseColor.png");
	s_Assets[Asset::snow_dragonMaterial] = AssetManager::CreateMaterial(s_Assets[Asset::snow_dragonTexture]);
	s_ModelPrefabs[Asset::snow_dragon] = Model::LoadSkinned(scene, "Model/enemy/snow_dragon/scene.gltf", { s_Assets[Asset::snow_dragonMaterial] });
	s_ModelPrefabs[Asset::snow_dragon].GetComponent<SkinnedMeshRendererComponent>().Enabled = false;
	//vdsb
	s_Assets[Asset::shadow_dragonTexture] = AssetManager::LoadTexture("Model/enemy/shadow_dragon/textures/efmat_npc_mst_228_fire_01_baseColor.png");
	s_Assets[Asset::shadow_dragonMaterial] = AssetManager::CreateMaterial(s_Assets[Asset::shadow_dragonTexture]);
	s_ModelPrefabs[Asset::shadow_dragon] = Model::LoadSkinned(scene,"Model/enemy/shadow_dragon/scene.gltf" , { s_Assets[Asset::shadow_dragonMaterial] });
	s_ModelPrefabs[Asset::shadow_dragon].GetComponent<SkinnedMeshRendererComponent>().Enabled = false;

	s_Assets[Asset::monster_skull_hight_polyTexture1] = AssetManager::LoadTexture("Model/enemy/monster_skull_hight_poly/textures/MI_M_B_43_Longhaikuilei_body_baseColor.png");
	s_Assets[Asset::monster_skull_hight_polyMaterial1] = AssetManager::CreateMaterial(s_Assets[Asset::monster_skull_hight_polyTexture1]);
	s_Assets[Asset::monster_skull_hight_polyTexture2] = AssetManager::LoadTexture("Model/enemy/monster_skull_hight_poly/textures/MI_M_B_43_Longhaikuilei_hand_baseColor.png");
	s_Assets[Asset::monster_skull_hight_polyMaterial2] = AssetManager::CreateMaterial(s_Assets[Asset::monster_skull_hight_polyTexture2]);
	s_Assets[Asset::monster_skull_hight_polyTexture3] = AssetManager::LoadTexture("Model/enemy/monster_skull_hight_poly/textures/MI_M_B_43_Longhaikuilei_head_baseColor.png");
	s_Assets[Asset::monster_skull_hight_polyMaterial3] = AssetManager::CreateMaterial(s_Assets[Asset::monster_skull_hight_polyTexture3]);
	s_ModelPrefabs[Asset::monster_skull_hight_poly] = Model::LoadSkinned(scene,"Model/enemy/monster_skull_hight_poly/scene.gltf" , { s_Assets[Asset::monster_skull_hight_polyMaterial1], s_Assets[Asset::monster_skull_hight_polyMaterial2], s_Assets[Asset::monster_skull_hight_polyMaterial3] });
	s_ModelPrefabs[Asset::monster_skull_hight_poly].GetComponent<SkinnedMeshRendererComponent>().Enabled = false;
	s_ModelPrefabs[Asset::monster_skull_hight_poly].GetComponent<TransformComponent>().Scale = glm::vec3(0.001f);

	s_Assets[Asset::monster_low_polyTexture] = AssetManager::LoadTexture("Model/enemy/monster_low_poly/textures/R27401_baseColor.png");
	s_Assets[Asset::monster_low_polyMaterial] = AssetManager::CreateMaterial(s_Assets[Asset::monster_low_polyTexture]);
	s_ModelPrefabs[Asset::monster_low_poly] = Model::LoadSkinned(scene,"Model/enemy/monster_low_poly/scene.gltf" , { s_Assets[Asset::monster_low_polyMaterial] });
	s_ModelPrefabs[Asset::monster_low_poly].GetComponent<SkinnedMeshRendererComponent>().Enabled = false;


	s_Assets[Asset::fantasy_reverse_horn_dragon_hight_polyTexture] = AssetManager::LoadTexture("Model/enemy/fantasy_reverse_horn_dragon_hight_poly/textures/Scene_-_Root_baseColor.png");
	s_Assets[Asset::fantasy_reverse_horn_dragon_hight_polyMaterial] = AssetManager::CreateMaterial(s_Assets[Asset::fantasy_reverse_horn_dragon_hight_polyTexture]);
	s_ModelPrefabs[Asset::fantasy_reverse_horn_dragon_hight_poly] = Model::LoadSkinned(scene,"Model/enemy/fantasy_reverse_horn_dragon_hight_poly/scene.gltf" , { s_Assets[Asset::fantasy_reverse_horn_dragon_hight_polyMaterial] });
	s_ModelPrefabs[Asset::fantasy_reverse_horn_dragon_hight_poly].GetComponent<SkinnedMeshRendererComponent>().Enabled = false;

	s_Assets[Asset::dragon_blue_low_polyTexture] = AssetManager::LoadTexture("Model/enemy/dragon_blue_-_low_poly/textures/m_13jzq_g_baseColor.png");
	s_Assets[Asset::dragon_blue_low_polyMaterial] = AssetManager::CreateMaterial(s_Assets[Asset::dragon_blue_low_polyTexture]);
	s_ModelPrefabs[Asset::dragon_blue_low_poly] = Model::LoadSkinned(scene, "Model/enemy/dragon_blue_-_low_poly/scene.gltf", { s_Assets[Asset::dragon_blue_low_polyMaterial] });
	s_ModelPrefabs[Asset::dragon_blue_low_poly].GetComponent<SkinnedMeshRendererComponent>().Enabled = false;

	s_Assets[Asset::baby_yoda_grogu_with_glassesTexture1] = AssetManager::LoadTexture("Model/enemy/baby_yoda_grogu_with_glasses/textures/Material.004_baseColor.jpeg");
	s_Assets[Asset::baby_yoda_grogu_with_glassesMaterial1] = AssetManager::CreateMaterial(s_Assets[Asset::baby_yoda_grogu_with_glassesTexture1]);
	s_Assets[Asset::baby_yoda_grogu_with_glassesTexture2] = AssetManager::LoadTexture("Model/enemy/baby_yoda_grogu_with_glasses/textures/Material.016_baseColor.png");
	s_Assets[Asset::baby_yoda_grogu_with_glassesMaterial2] = AssetManager::CreateMaterial(s_Assets[Asset::baby_yoda_grogu_with_glassesTexture2]);
	s_Assets[Asset::baby_yoda_grogu_with_glassesTexture3] = AssetManager::LoadTexture("Model/enemy/baby_yoda_grogu_with_glasses/textures/Material_baseColor.png");
	s_Assets[Asset::baby_yoda_grogu_with_glassesMaterial3] = AssetManager::CreateMaterial(s_Assets[Asset::baby_yoda_grogu_with_glassesTexture3]);
	s_Assets[Asset::baby_yoda_grogu_with_glassesTexture4] = AssetManager::LoadTexture("Model/enemy/baby_yoda_grogu_with_glasses/textures/black.png");
	s_Assets[Asset::baby_yoda_grogu_with_glassesMaterial4] = AssetManager::CreateMaterial(s_Assets[Asset::baby_yoda_grogu_with_glassesTexture4]);
	s_ModelPrefabs[Asset::baby_yoda_grogu_with_glasses] = Model::Load(scene, "Model/enemy/baby_yoda_grogu_with_glasses/untitled.dae", { s_Assets[Asset::baby_yoda_grogu_with_glassesMaterial4],s_Assets[Asset::baby_yoda_grogu_with_glassesMaterial2],s_Assets[Asset::baby_yoda_grogu_with_glassesMaterial4],s_Assets[Asset::baby_yoda_grogu_with_glassesMaterial1],s_Assets[Asset::baby_yoda_grogu_with_glassesMaterial2] }); //1eye2hand3glasses4clothes5face
	s_ModelPrefabs[Asset::baby_yoda_grogu_with_glasses].GetComponent<MeshRendererComponent>().Enabled = false;
	//Model::LoadSkinned()
	//SkinnedMeshRendererComponent
	for (int i = (int)Asset::ArcherTowerLevel1Model; i <= (int)Asset::WizardTowerLevel4Model; i++)
	{
		s_ModelPrefabs[(Asset)i].GetComponent<MeshRendererComponent>().Enabled = false;
		s_ModelPrefabs[(Asset)i].GetComponent<TransformComponent>().Scale = glm::vec3(0.1f);
	}


	s_Assets[Asset::OpenSansFont] = AssetManager::LoadFont("Fonts/OpenSans-Regular.ttf");

	s_Scene = scene;
}

void AssetLibrary::ShutDown()
{
	for (int i = (int)Asset::ArcherTowerLevel1Model; i <= (int)Asset::WizardTowerLevel4Model; i++)
	{
		AssetManager::UnloadMesh(s_ModelPrefabs[(Asset)i].GetComponent<MeshRendererComponent>().Mesh);
	}

	AssetManager::UnloadMaterial(s_Assets[Asset::TowerMaterial]);
	AssetManager::UnloadTexture(s_Assets[Asset::TowerTexture]);

	AssetManager::UnloadFont(s_Assets[Asset::OpenSansFont]);
}

AssetHandle AssetLibrary::Get(Asset asset)
{
	return s_Assets[asset];
}

Entity AssetLibrary::GetModel(Asset asset)
{
	return s_Scene->CreateEntity(s_ModelPrefabs[asset]);
}