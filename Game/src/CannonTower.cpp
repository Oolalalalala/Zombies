#include "CannonTower.h"

CannonTower::CannonTower(Ref<Scene> scene)
	:Tower(scene, 100.0, 100.0, Asset::CannonTowerLevel1Model)
{
}
void CannonTower::changeModel(Ref<Scene> scene)
{
	TransformComponent oldTransform = _entity.GetComponent<TransformComponent>();

	switch (_level)
	{
	case 1:
		Destroy(scene);
		_entity = AssetLibrary::GetModel(Asset::CannonTowerLevel1Model);
		break;
	case 2:
		Destroy(scene);
		_entity = AssetLibrary::GetModel(Asset::CannonTowerLevel2Model);
		break;
	case 3:
		Destroy(scene);
		_entity = AssetLibrary::GetModel(Asset::CannonTowerLevel3Model);
		break;
	case 4:
		Destroy(scene);
		_entity = AssetLibrary::GetModel(Asset::CannonTowerLevel4Model);
		break;
	}
	_entity.GetComponent<TransformComponent>() = oldTransform;
}