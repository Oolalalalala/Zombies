#include "ArcherTower.h"

ArcherTower::ArcherTower(Ref<Scene> scene)
	:Tower(scene, 100.0, 100.0, Asset::ArcherTowerLevel1Model)
{
}
void ArcherTower::changeModel(Ref<Scene> scene)
{
	TransformComponent oldTransform = _entity.GetComponent<TransformComponent>();

	switch (_level)
	{
	case 1:
		Destroy(scene);
		_entity = AssetLibrary::GetModel(Asset::ArcherTowerLevel1Model);
		break;
	case 2:
		Destroy(scene);
		_entity = AssetLibrary::GetModel(Asset::ArcherTowerLevel2Model);
		break;
	case 3:
		Destroy(scene);
		_entity = AssetLibrary::GetModel(Asset::ArcherTowerLevel3Model);
		break;
	case 4:
		Destroy(scene);
		_entity = AssetLibrary::GetModel(Asset::ArcherTowerLevel4Model);
		break;
	}

	_entity.GetComponent<TransformComponent>() = oldTransform;
}