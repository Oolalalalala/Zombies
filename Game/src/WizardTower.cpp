#include "WizardTower.h"

WizardTower::WizardTower(Ref<Scene> scene)
	:Tower(scene, 100.0, 100.0, Asset::WizardTowerLevel1Model)
{
}
void WizardTower::changeModel(Ref<Scene> scene)
{
	TransformComponent oldTransform = _entity.GetComponent<TransformComponent>();

	switch (_level)
	{
	case 1:
		Destroy(scene);
		_entity = AssetLibrary::GetModel(Asset::WizardTowerLevel1Model);
		break;
	case 2:
		Destroy(scene);
		_entity = AssetLibrary::GetModel(Asset::WizardTowerLevel2Model);
		break;
	case 3:
		Destroy(scene);
		_entity = AssetLibrary::GetModel(Asset::WizardTowerLevel3Model);
		break;
	case 4:
		Destroy(scene);
		_entity = AssetLibrary::GetModel(Asset::WizardTowerLevel4Model);
		break;
	}
	_entity.GetComponent<TransformComponent>() = oldTransform;
}