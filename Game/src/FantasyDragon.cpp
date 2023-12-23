#include "FantasyDragon.h"

FantasyDragon::FantasyDragon(Ref<Scene> scene)
	: Enemy(scene, 100.0, 100.0, Asset::fantasy_reverse_horn_dragon_hight_poly)
{
	_entity.GetComponent<TransformComponent>().Scale *= 100.0f;
	_entity.GetComponent<AnimatorComponent>().IsPlaying = true;
}