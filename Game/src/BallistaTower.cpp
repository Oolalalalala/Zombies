#include "BallistaTower.h"

BallistaTower::BallistaTower(Ref<Scene> scene)
	:Tower(scene, 100.0, 100.0, Asset::BallistaTowerLevel1Model)
{
}
void BallistaTower::changeModel(Ref<Scene> scene)
{
	TransformComponent oldTransform = _entity.GetComponent<TransformComponent>();

	switch (_level)
	{
	case 1:
		Destroy(scene);
		_entity = AssetLibrary::GetModel(Asset::BallistaTowerLevel1Model);
		break;
	case 2:
		Destroy(scene);
		_entity = AssetLibrary::GetModel(Asset::BallistaTowerLevel2Model);
		break;
	case 3:
		Destroy(scene);
		_entity = AssetLibrary::GetModel(Asset::BallistaTowerLevel3Model);
		break;
	case 4:
		Destroy(scene);
		_entity = AssetLibrary::GetModel(Asset::BallistaTowerLevel4Model);
		break;
	}
	_entity.GetComponent<TransformComponent>() = oldTransform;
}