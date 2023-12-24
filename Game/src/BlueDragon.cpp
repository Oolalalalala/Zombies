#include "BlueDragon.h"

BlueDragon::BlueDragon(Ref<Scene> scene,int level)
	: Enemy(scene, level, Asset::dragon_blue_low_poly)
{
	_entity.GetComponent<AnimatorComponent>().IsPlaying = true;
}