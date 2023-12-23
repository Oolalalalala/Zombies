#include "AssetLibrary.h"


Ref<Scene> AssetLibrary::s_Scene;
std::unordered_map<Asset, AssetHandle> AssetLibrary::s_Assets;
std::unordered_map<Asset, Entity> AssetLibrary::s_ModelPrefabs;


void AssetLibrary::Initialize(Ref<Scene> scene)
{
	AssetManager::SetAssetDirectory("asset");

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
