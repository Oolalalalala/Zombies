#pragma once
#include "Enemy.h"
class WhiteChineseDragon :public Enemy
{
public:
	WhiteChineseDragon() = default;
	WhiteChineseDragon(Ref<Scene> scene);
};