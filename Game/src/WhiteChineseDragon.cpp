#include "WhiteChineseDragon.h"

WhiteChineseDragon::WhiteChineseDragon(Ref<Scene> scene)
	: Enemy(scene, 100.0, 100.0, Asset::white_chinese_dragon)
{
	_entity.GetComponent<TransformComponent>().Scale *= 100.0f;
	_entity.GetComponent<AnimatorComponent>().IsPlaying=true;
}