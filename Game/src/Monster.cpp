#include "Monster.h"

Monster::Monster(Ref<Scene> scene,int level)
	: Enemy(scene, level, Asset::monster_low_poly)
{
	_entity.GetComponent<AnimatorComponent>().IsPlaying = true;
}