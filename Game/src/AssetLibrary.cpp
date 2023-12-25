#include "AssetLibrary.h"


Ref<Scene> AssetLibrary::s_Scene;
std::unordered_map<Asset, AssetHandle> AssetLibrary::s_Assets;
std::unordered_map<Asset, Entity> AssetLibrary::s_ModelPrefabs;


void AssetLibrary::Initialize(Ref<Scene> scene)
{
	AssetManager::SetAssetDirectory("asset");

	

	
	//TOWERS
	s_Assets[Asset::TowerTexture] = AssetManager::LoadTexture("Model/towers/texture/Texture_MAp_fortress_elves.png");
	s_Assets[Asset::TowerMaterial] = AssetManager::CreateMaterial(s_Assets[Asset::TowerTexture]);
	s_Assets[Asset::TowerTransparentMaterial] = AssetManager::CreateMaterial(s_Assets[Asset::TowerTexture]);
	AssetManager::GetMaterial(s_Assets[Asset::TowerTransparentMaterial])->Color.a = 0.3f;

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
	//ENEMIES
	//ENEMIES
	//ENEMIES
	//ENEMIES
	//ENEMIES
	//ENEMIES

	s_Assets[Asset::PatrickTexture] = AssetManager::LoadTexture("Model/enemy/spongebob_squarepants_patrick/textures/MI_Patrick_PBR_baseColor.png");
	s_Assets[Asset::PatrickMaterial] = AssetManager::CreateMaterial(s_Assets[Asset::PatrickTexture]);
	s_ModelPrefabs[Asset::spongebob_squarepants_patrick] = Model::Load(scene, "Model/enemy/spongebob_squarepants_patrick/scene.gltf", { s_Assets[Asset::PatrickMaterial] });
	s_ModelPrefabs[Asset::spongebob_squarepants_patrick].GetComponent<MeshRendererComponent>().Enabled = false;
	s_ModelPrefabs[Asset::spongebob_squarepants_patrick].GetComponent<TransformComponent>().Scale *= 100.0f;
	//s_ModelPrefabs[Asset::monster_skull_hight_poly].GetComponent<TransformComponent>().Rotation = glm::rotate(s_ModelPrefabs[Asset::monster_skull_hight_poly].GetComponent<TransformComponent>().Rotation, glm::radians(270.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	s_Assets[Asset::white_chinese_dragonTexture] = AssetManager::LoadTexture("Model/enemy/white_chinese_dragon/textures/BaiLongZuoQi_baseColor.png");
	s_Assets[Asset::white_chinese_dragonMaterial] = AssetManager::CreateMaterial(s_Assets[Asset::white_chinese_dragonTexture]);
	s_ModelPrefabs[Asset::white_chinese_dragon] = Model::LoadSkinned(scene, "Model/enemy/white_chinese_dragon/scene.gltf", { s_Assets[Asset::white_chinese_dragonMaterial] });
	s_ModelPrefabs[Asset::white_chinese_dragon].GetComponent<SkinnedMeshRendererComponent>().Enabled = false;
	s_ModelPrefabs[Asset::white_chinese_dragon].GetComponent<TransformComponent>().Scale *= 50.0f;
	//s_ModelPrefabs[Asset::monster_skull_hight_poly].GetComponent<TransformComponent>().Rotation = glm::rotate(s_ModelPrefabs[Asset::monster_skull_hight_poly].GetComponent<TransformComponent>().Rotation, glm::radians(270.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	s_Assets[Asset::snow_dragonTexture] = AssetManager::LoadTexture("Model/enemy/snow_dragon/textures/zq127_binglong_1_baseColor.png");
	s_Assets[Asset::snow_dragonMaterial] = AssetManager::CreateMaterial(s_Assets[Asset::snow_dragonTexture]);
	s_ModelPrefabs[Asset::snow_dragon] = Model::LoadSkinned(scene, "Model/enemy/snow_dragon/scene.gltf", { s_Assets[Asset::snow_dragonMaterial] });
	s_ModelPrefabs[Asset::snow_dragon].GetComponent<SkinnedMeshRendererComponent>().Enabled = false;
	s_ModelPrefabs[Asset::snow_dragon].GetComponent<TransformComponent>().Scale *= 35.0f;
	//s_ModelPrefabs[Asset::monster_skull_hight_poly].GetComponent<TransformComponent>().Rotation = glm::rotate(s_ModelPrefabs[Asset::monster_skull_hight_poly].GetComponent<TransformComponent>().Rotation, glm::radians(270.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	s_Assets[Asset::shadow_dragonTexture] = AssetManager::LoadTexture("Model/enemy/shadow_dragon/textures/efmat_npc_mst_228_fire_01_baseColor.png");
	s_Assets[Asset::shadow_dragonMaterial] = AssetManager::CreateMaterial(s_Assets[Asset::shadow_dragonTexture]);
	s_ModelPrefabs[Asset::shadow_dragon] = Model::LoadSkinned(scene, "Model/enemy/shadow_dragon/scene.gltf", { s_Assets[Asset::shadow_dragonMaterial] });
	s_ModelPrefabs[Asset::shadow_dragon].GetComponent<SkinnedMeshRendererComponent>().Enabled = false;
	s_ModelPrefabs[Asset::shadow_dragon].GetComponent<TransformComponent>().Scale *= 35.0f;
	//s_ModelPrefabs[Asset::shadow_dragon].GetComponent<TransformComponent>().Rotation = glm::rotate(s_ModelPrefabs[Asset::shadow_dragon].GetComponent<TransformComponent>().Rotation, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	s_ModelPrefabs[Asset::shadow_dragon].GetComponent<TransformComponent>().Position = { 0,50,0 };

	s_Assets[Asset::monster_skull_hight_polyTexture1] = AssetManager::LoadTexture("Model/enemy/monster_skull_hight_poly/textures/MI_M_B_43_Longhaikuilei_body_baseColor.png");
	s_Assets[Asset::monster_skull_hight_polyMaterial1] = AssetManager::CreateMaterial(s_Assets[Asset::monster_skull_hight_polyTexture1]);
	s_Assets[Asset::monster_skull_hight_polyTexture2] = AssetManager::LoadTexture("Model/enemy/monster_skull_hight_poly/textures/MI_M_B_43_Longhaikuilei_hand_baseColor.png");
	s_Assets[Asset::monster_skull_hight_polyMaterial2] = AssetManager::CreateMaterial(s_Assets[Asset::monster_skull_hight_polyTexture2]);
	s_Assets[Asset::monster_skull_hight_polyTexture3] = AssetManager::LoadTexture("Model/enemy/monster_skull_hight_poly/textures/MI_M_B_43_Longhaikuilei_head_baseColor.png");
	s_Assets[Asset::monster_skull_hight_polyMaterial3] = AssetManager::CreateMaterial(s_Assets[Asset::monster_skull_hight_polyTexture3]);
	s_ModelPrefabs[Asset::monster_skull_hight_poly] = Model::LoadSkinned(scene, "Model/enemy/monster_skull_hight_poly/scene.gltf", { s_Assets[Asset::monster_skull_hight_polyMaterial1], s_Assets[Asset::monster_skull_hight_polyMaterial2], s_Assets[Asset::monster_skull_hight_polyMaterial3] });
	s_ModelPrefabs[Asset::monster_skull_hight_poly].GetComponent<SkinnedMeshRendererComponent>().Enabled = false;
	s_ModelPrefabs[Asset::monster_skull_hight_poly].GetComponent<TransformComponent>().Scale = glm::vec3(0.001f);
	s_ModelPrefabs[Asset::monster_skull_hight_poly].GetComponent<TransformComponent>().Scale *= 70.0f;
	//s_ModelPrefabs[Asset::monster_skull_hight_poly].GetComponent<TransformComponent>().Rotation = glm::rotate(s_ModelPrefabs[Asset::monster_skull_hight_poly].GetComponent<TransformComponent>().Rotation, glm::radians(270.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	s_ModelPrefabs[Asset::monster_skull_hight_poly].GetComponent<TransformComponent>().Position = { 0,50,0 };

	s_Assets[Asset::monster_low_polyTexture] = AssetManager::LoadTexture("Model/enemy/monster_low_poly/textures/R27401_baseColor.png");
	s_Assets[Asset::monster_low_polyMaterial] = AssetManager::CreateMaterial(s_Assets[Asset::monster_low_polyTexture]);
	s_ModelPrefabs[Asset::monster_low_poly] = Model::LoadSkinned(scene, "Model/enemy/monster_low_poly/scene.gltf", { s_Assets[Asset::monster_low_polyMaterial] });
	s_ModelPrefabs[Asset::monster_low_poly].GetComponent<SkinnedMeshRendererComponent>().Enabled = false;
	s_ModelPrefabs[Asset::monster_low_poly].GetComponent<TransformComponent>().Scale *= 20.0f;
	//s_ModelPrefabs[Asset::monster_low_poly].GetComponent<TransformComponent>().Rotation = glm::rotate(s_ModelPrefabs[Asset::monster_low_poly].GetComponent<TransformComponent>().Rotation, glm::radians(270.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	s_ModelPrefabs[Asset::monster_low_poly].GetComponent<TransformComponent>().Position = { 0,50,0 };

	s_Assets[Asset::dragon_blue_low_polyTexture] = AssetManager::LoadTexture("Model/enemy/dragon_blue_-_low_poly/textures/m_13jzq_g_baseColor.png");
	s_Assets[Asset::dragon_blue_low_polyMaterial] = AssetManager::CreateMaterial(s_Assets[Asset::dragon_blue_low_polyTexture]);
	s_ModelPrefabs[Asset::dragon_blue_low_poly] = Model::LoadSkinned(scene, "Model/enemy/dragon_blue_-_low_poly/scene.gltf", { s_Assets[Asset::dragon_blue_low_polyMaterial] });
	s_ModelPrefabs[Asset::dragon_blue_low_poly].GetComponent<SkinnedMeshRendererComponent>().Enabled = false;
	s_ModelPrefabs[Asset::dragon_blue_low_poly].GetComponent<TransformComponent>().Scale *= 10.0f;
	//s_ModelPrefabs[Asset::dragon_blue_low_poly].GetComponent<TransformComponent>().Rotation = glm::rotate(s_ModelPrefabs[Asset::dragon_blue_low_poly].GetComponent<TransformComponent>().Rotation, glm::radians(270.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	s_ModelPrefabs[Asset::dragon_blue_low_poly].GetComponent<TransformComponent>().Position = { 0,10,0 };

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
	s_ModelPrefabs[Asset::baby_yoda_grogu_with_glasses].GetComponent<TransformComponent>().Scale *= 50.0f;
	//s_ModelPrefabs[Asset::baby_yoda_grogu_with_glasses].GetComponent<TransformComponent>().Rotation = glm::rotate(s_ModelPrefabs[Asset::baby_yoda_grogu_with_glasses].GetComponent<TransformComponent>().Rotation, glm::radians(270.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	s_ModelPrefabs[Asset::baby_yoda_grogu_with_glasses].GetComponent<TransformComponent>().Position = { 0,80,0 };

	//FLOOR
	//FLOOR
	//FLOOR
	//FLOOR
	//FLOOR
	//FLOOR
	s_Assets[Asset::MobPathTexture] = AssetManager::LoadTexture("Texture/monsfloor.png");
	s_Assets[Asset::OringinalFloor] = AssetManager::LoadTexture("Texture/originalfloor.png");
	//CRYSTAL
	s_Assets[Asset::stylized_crystalTexture] = AssetManager::LoadTexture("Model/stylized_crystal/textures/material_baseColor.jpeg");
	s_Assets[Asset::stylized_crystalMaterial] = AssetManager::CreateMaterial(s_Assets[Asset::stylized_crystalTexture]);
	s_ModelPrefabs[Asset::stylized_crystal] = Model::Load(scene, "Model/stylized_crystal/scene.gltf", { s_Assets[Asset::stylized_crystalMaterial] });
	s_ModelPrefabs[Asset::stylized_crystal].GetComponent<MeshRendererComponent>().Enabled = false;
	
	//Model::LoadSkinned()
	//SkinnedMeshRendererComponent



	s_Assets[Asset::ArrowTexture] = AssetManager::LoadTexture("Model/weapons/arrow/textures/Material.001_Base_Color_1001.png");
	s_Assets[Asset::ArrowMaterial] = AssetManager::CreateMaterial(s_Assets[Asset::ArrowTexture]);
	s_ModelPrefabs[Asset::ArrowModel] = Model::Load(scene, "Model/weapons/arrow/source/Arrow.fbx", { s_Assets[Asset::ArrowMaterial] });
	s_ModelPrefabs[Asset::ArrowModel].GetComponent<MeshRendererComponent>().Enabled = false;
	s_ModelPrefabs[Asset::ArrowModel].GetComponent<TransformComponent>().Scale = glm::vec3(300.0f, 100.0f, 300.0f);


	s_Assets[Asset::CannonBallTexture] = AssetManager::LoadTexture("Model/weapons/cannonball/textures/Bomba_Base_Color.png");
	s_Assets[Asset::CannonBallMaterial] = AssetManager::CreateMaterial(s_Assets[Asset::CannonBallTexture]);
	s_ModelPrefabs[Asset::CannonBallModel] = Model::Load(scene, "Model/weapons/cannonball/source/Bomba.fbx", { s_Assets[Asset::CannonBallMaterial] });
	s_ModelPrefabs[Asset::CannonBallModel].GetComponent<MeshRendererComponent>().Enabled = false;
	s_ModelPrefabs[Asset::CannonBallModel].GetComponent<TransformComponent>().Scale *= 0.01f;


	s_Assets[Asset::MageBallTexture] = AssetManager::LoadTexture("Model/weapons/mageball/BaseColor.png");
	s_Assets[Asset::MageBallMaterial] = AssetManager::CreateMaterial(s_Assets[Asset::MageBallTexture]);
	s_ModelPrefabs[Asset::MageBallModel] = Model::Load(scene, "Model/weapons/mageball/mageBall.fbx", { s_Assets[Asset::MageBallMaterial] });
	s_ModelPrefabs[Asset::MageBallModel].GetComponent<MeshRendererComponent>().Enabled = false;
	s_ModelPrefabs[Asset::MageBallModel].GetComponent<TransformComponent>().Scale *= 0.01f;
	
	s_Assets[Asset::LightBeamTexture] = AssetManager::LoadTexture("Model/weapons/lightbeam/base_color.png");
	s_Assets[Asset::LightBeamMaterial] = AssetManager::CreateMaterial(s_Assets[Asset::LightBeamTexture]);
	s_ModelPrefabs[Asset::LightBeamModel] = Model::Load(scene, "Model/weapons/lightbeam/light_beam.fbx", { s_Assets[Asset::LightBeamMaterial] });
	s_ModelPrefabs[Asset::LightBeamModel].GetComponent<MeshRendererComponent>().Enabled = false;


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


		
	AssetManager::UnloadMesh(s_ModelPrefabs[Asset::spongebob_squarepants_patrick].GetComponent<MeshRendererComponent>().Mesh);
	AssetManager::UnloadMaterial(s_Assets[Asset::PatrickMaterial]);
	AssetManager::UnloadTexture(s_Assets[Asset::PatrickTexture]);
	
	AssetManager::UnloadMesh(s_ModelPrefabs[Asset::baby_yoda_grogu_with_glasses].GetComponent<MeshRendererComponent>().Mesh);
	AssetManager::UnloadMaterial(s_Assets[Asset::baby_yoda_grogu_with_glassesMaterial1]);
	AssetManager::UnloadTexture(s_Assets[Asset::baby_yoda_grogu_with_glassesTexture1]);
	AssetManager::UnloadMaterial(s_Assets[Asset::baby_yoda_grogu_with_glassesMaterial2]);
	AssetManager::UnloadTexture(s_Assets[Asset::baby_yoda_grogu_with_glassesTexture2]); 
	AssetManager::UnloadMaterial(s_Assets[Asset::baby_yoda_grogu_with_glassesMaterial3]);
	AssetManager::UnloadTexture(s_Assets[Asset::baby_yoda_grogu_with_glassesTexture3]);
	
	AssetManager::UnloadMesh(s_ModelPrefabs[Asset::ArrowModel].GetComponent<MeshRendererComponent>().Mesh);
	AssetManager::UnloadMaterial(s_Assets[Asset::ArrowMaterial]);
	AssetManager::UnloadTexture(s_Assets[Asset::ArrowTexture]);
	
	AssetManager::UnloadMesh(s_ModelPrefabs[Asset::CannonBallModel].GetComponent<MeshRendererComponent>().Mesh);
	AssetManager::UnloadMaterial(s_Assets[Asset::CannonBallMaterial]);
	AssetManager::UnloadTexture(s_Assets[Asset::CannonBallTexture]);
	
	AssetManager::UnloadMesh(s_ModelPrefabs[Asset::MageBallModel].GetComponent<MeshRendererComponent>().Mesh);
	AssetManager::UnloadMaterial(s_Assets[Asset::MageBallMaterial]);
	AssetManager::UnloadTexture(s_Assets[Asset::MageBallTexture]);
	
	AssetManager::UnloadMesh(s_ModelPrefabs[Asset::LightBeamModel].GetComponent<MeshRendererComponent>().Mesh);
	AssetManager::UnloadMaterial(s_Assets[Asset::LightBeamMaterial]);
	AssetManager::UnloadTexture(s_Assets[Asset::LightBeamTexture]);
	
	AssetManager::UnloadMesh(s_ModelPrefabs[Asset::stylized_crystal].GetComponent<MeshRendererComponent>().Mesh);
	AssetManager::UnloadMaterial(s_Assets[Asset::stylized_crystalMaterial]);
	AssetManager::UnloadTexture(s_Assets[Asset::stylized_crystalTexture]);
	
	AssetManager::UnloadTexture(s_Assets[Asset::MobPathTexture]);
	AssetManager::UnloadTexture(s_Assets[Asset::OringinalFloor]);
	
	AssetManager::UnloadSkinnedMesh(s_ModelPrefabs[Asset::white_chinese_dragon].GetComponent<SkinnedMeshRendererComponent>().SkinnedMesh);
	AssetManager::UnloadMaterial(s_Assets[Asset::white_chinese_dragonMaterial]);
	AssetManager::UnloadTexture(s_Assets[Asset::white_chinese_dragonTexture]);
	
	AssetManager::UnloadSkinnedMesh(s_ModelPrefabs[Asset::snow_dragon].GetComponent<SkinnedMeshRendererComponent>().SkinnedMesh);
	AssetManager::UnloadTexture(s_Assets[Asset::snow_dragonTexture]);
	AssetManager::UnloadMaterial(s_Assets[Asset::snow_dragonMaterial]);
	
	AssetManager::UnloadSkinnedMesh(s_ModelPrefabs[Asset::shadow_dragon].GetComponent<SkinnedMeshRendererComponent>().SkinnedMesh);
	AssetManager::UnloadTexture(s_Assets[Asset::shadow_dragonTexture]);
	AssetManager::UnloadMaterial(s_Assets[Asset::shadow_dragonMaterial]);
	
	AssetManager::UnloadSkinnedMesh(s_ModelPrefabs[Asset::monster_low_poly].GetComponent<SkinnedMeshRendererComponent>().SkinnedMesh);
	AssetManager::UnloadTexture(s_Assets[Asset::monster_low_polyTexture]);
	AssetManager::UnloadMaterial(s_Assets[Asset::monster_low_polyMaterial]);
	
	AssetManager::UnloadSkinnedMesh(s_ModelPrefabs[Asset::dragon_blue_low_poly].GetComponent<SkinnedMeshRendererComponent>().SkinnedMesh);
	AssetManager::UnloadTexture(s_Assets[Asset::dragon_blue_low_polyTexture]);
	AssetManager::UnloadMaterial(s_Assets[Asset::dragon_blue_low_polyMaterial]);
	
	AssetManager::UnloadSkinnedMesh(s_ModelPrefabs[Asset::monster_skull_hight_poly].GetComponent<SkinnedMeshRendererComponent>().SkinnedMesh);
	AssetManager::UnloadTexture(s_Assets[Asset::monster_skull_hight_polyTexture1]);
	AssetManager::UnloadMaterial(s_Assets[Asset::monster_skull_hight_polyMaterial1]);
	AssetManager::UnloadTexture(s_Assets[Asset::monster_skull_hight_polyTexture2]);
	AssetManager::UnloadMaterial(s_Assets[Asset::monster_skull_hight_polyMaterial2]);
	AssetManager::UnloadTexture(s_Assets[Asset::monster_skull_hight_polyTexture3]);
	AssetManager::UnloadMaterial(s_Assets[Asset::monster_skull_hight_polyMaterial3]);
}

AssetHandle AssetLibrary::Get(Asset asset)
{
	return s_Assets[asset];
}

void AssetLibrary::DestoryModel(Entity model)
{
	s_Scene->DestroyEntity(model);
}

Entity AssetLibrary::GetModel(Asset asset)
{
	return s_Scene->CreateEntity(s_ModelPrefabs[asset]);
}
