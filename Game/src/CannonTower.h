#pragma once
#include "Tower.h"

struct CannonAttackTarget
{
	Enemy* Target;
	glm::vec3 FallbackPosition;
};

class CannonTower :public Tower
{
public:
	CannonTower() = default;
	CannonTower(Ref<Scene> scene);
	~CannonTower();

	void OnUpdate(float dt) override;
	void AddTrackingEnemy(Enemy* target) override;
	void RemoveTrackingEnemy(Enemy* target) override;

protected:
	void changeModel(Ref<Scene> scene);

	std::vector<Enemy*> m_TargetList;
	std::unordered_map<Entity, CannonAttackTarget> m_AttackList;

	float m_FireInterval = 0.7f;
	float m_CannonBallSpeed = 300.0f;
	float m_Timer = 0.0f;
	const static glm::vec3 s_AttackPointOffset;
};