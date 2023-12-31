#pragma once
#include "Tower.h"

struct ArcherAttackTarget
{
	Enemy* Target;
	glm::vec3 FallbackPosition;
};

class ArcherTower :public Tower
{
public:
	ArcherTower() = default;
	ArcherTower(Ref<Scene> scene);
	~ArcherTower();

	void OnUpdate(float dt) override;
	void AddTrackingEnemy(Enemy* target) override;
	void RemoveTrackingEnemy(Enemy* target) override;

protected:
	void changeModel(Ref<Scene> scene);

	std::vector<Enemy*> m_TargetList;
	std::unordered_map<Entity, ArcherAttackTarget> m_AttackList;

	float m_FireInterval = 0.7f;
	float m_ArrowSpeed = 600.0f;
	float m_Timer = 0.0f;
	const static glm::vec3 s_AttackPointOffset;
};