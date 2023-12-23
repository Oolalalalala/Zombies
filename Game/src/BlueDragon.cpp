#include "BlueDragon.h"

BlueDragon::BlueDragon(Ref<Scene> scene)
	: Enemy(scene, 100.0, 100.0, Asset::dragon_blue_low_poly)
{
	_entity.GetComponent<TransformComponent>().Scale *= 100.0f;
	_entity.GetComponent<AnimatorComponent>().IsPlaying = true;
}