#include "ShadowDragon.h"

ShadowDragon::ShadowDragon(Ref<Scene> scene)
	: Enemy(scene, 100.0, 100.0, Asset::shadow_dragon)
{
	_entity.GetComponent<TransformComponent>().Scale *= 100.0f;
	_entity.GetComponent<AnimatorComponent>().IsPlaying = true;
}