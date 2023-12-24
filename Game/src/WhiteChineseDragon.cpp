#include "WhiteChineseDragon.h"

WhiteChineseDragon::WhiteChineseDragon(Ref<Scene> scene,int level)
	: Enemy(scene, level, Asset::white_chinese_dragon)
{
	
	_entity.GetComponent<AnimatorComponent>().IsPlaying=true;
}