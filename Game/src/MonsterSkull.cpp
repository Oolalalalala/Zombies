#include "MonsterSkull.h"

MonsterSkull::MonsterSkull(Ref<Scene> scene,int level)
	: Enemy(scene,level, Asset::monster_skull_hight_poly)
{
	_entity.GetComponent<AnimatorComponent>().IsPlaying = true;
}