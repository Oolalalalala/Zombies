#pragma once
#include "Tower.h"
class BallistaTower :public Tower
{
public:
	BallistaTower() = default;
	BallistaTower(Ref<Scene> scene);

	void OnUpdate(float dt) override;
	void AddTrackingEnemy(Enemy* target) override;
	void RemoveTrackingEnemy(Enemy* target) override;

protected:
	void changeModel(Ref<Scene> scene);

	std::vector<Enemy*> m_TargetList;
	std::unordered_map<Entity, Enemy*> m_AttackList;

	float m_FireInterval = 0.15f;
	float m_ArrowSpeed = 400.0f;
	float m_Timer = 0.0f;
	const static glm::vec3 s_AttackPointOffset;
};