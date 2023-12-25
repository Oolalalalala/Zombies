#pragma once
#include "Tower.h"

struct BallistaAttackTarget
{
	Enemy* Target;
	glm::vec3 FallbackPosition;
};


class BallistaTower :public Tower
{
public:
	BallistaTower() = default;
	BallistaTower(Ref<Scene> scene);
	~BallistaTower();

	void OnUpdate(float dt) override;
	void AddTrackingEnemy(Enemy* target) override;
	void RemoveTrackingEnemy(Enemy* target) override;

protected:
	void changeModel(Ref<Scene> scene);

	std::vector<Enemy*> m_TargetList;
	std::unordered_map<Entity, BallistaAttackTarget> m_AttackList;

	float m_FireInterval = 0.15f;
	float m_ArrowSpeed = 500.0f;
	float m_Timer = 0.0f;
	const static glm::vec3 s_AttackPointOffset;
};