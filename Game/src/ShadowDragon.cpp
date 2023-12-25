#include "ShadowDragon.h"

ShadowDragon::ShadowDragon(Ref<Scene> scene,int level)
	: Enemy(scene, level, Asset::shadow_dragon)
{
	_entity.GetComponent<AnimatorComponent>().IsPlaying = true;
	_entity.GetComponent<AnimatorComponent>().AnimationIndex = 1;
}