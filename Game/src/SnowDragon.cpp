#include "SnowDragon.h"

SnowDragon::SnowDragon(Ref<Scene> scene,int level)
	: Enemy(scene, level, Asset::snow_dragon)
{
	_entity.GetComponent<AnimatorComponent>().IsPlaying = true;
	_entity.GetComponent<AnimatorComponent>().AnimationIndex = 1;
}