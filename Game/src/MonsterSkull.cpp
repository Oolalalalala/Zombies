#include "MonsterSkull.h"

MonsterSkull::MonsterSkull(Ref<Scene> scene)
	: Enemy(scene, 100.0, 100.0, Asset::monster_skull_hight_poly)
{
	_entity.GetComponent<TransformComponent>().Scale *= 100.0f;
	_entity.GetComponent<AnimatorComponent>().IsPlaying = true;
}