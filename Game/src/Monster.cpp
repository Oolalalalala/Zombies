#include "Monster.h"

Monster::Monster(Ref<Scene> scene)
	: Enemy(scene, 100.0, 100.0, Asset::monster_low_poly)
{
	_entity.GetComponent<TransformComponent>().Scale *= 100.0f;
	_entity.GetComponent<AnimatorComponent>().IsPlaying = true;
}