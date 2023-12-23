#include "PoisonTower.h"

PoisonTower::PoisonTower(Ref<Scene> scene)
	:Tower(scene, 100.0, 100.0, Asset::PoisonTowerLevel1Model)
{
}
void PoisonTower::changeModel(Ref<Scene> scene)
{
	TransformComponent oldTransform = _entity.GetComponent<TransformComponent>();

	switch (_level)
	{
	case 1:
		Destroy(scene);
		_entity = AssetLibrary::GetModel(Asset::PoisonTowerLevel1Model);
		break;
	case 2:
		Destroy(scene);
		_entity = AssetLibrary::GetModel(Asset::PoisonTowerLevel2Model);
		break;
	case 3:
		Destroy(scene);
		_entity = AssetLibrary::GetModel(Asset::PoisonTowerLevel3Model);
		break;
	case 4:
		Destroy(scene);
		_entity = AssetLibrary::GetModel(Asset::PoisonTowerLevel4Model);
		break;
	}
	_entity.GetComponent<TransformComponent>() = oldTransform;
}