#include "Patrick.h"

Patrick::Patrick(Ref<Scene> scene)
	: Enemy(scene,100.0,100.0, Asset::spongebob_squarepants_patrick)
{
	_entity.GetComponent<TransformComponent>().Scale *= 100.0f;
}
