#pragma once
#include "Tower.h"

struct MageBallData
{
	Enemy* Enemy;
	glm::vec3 Velocity;
	float AirTime = 0;
	glm::vec3 FallbackPosition;
};


class PoisonTower :public Tower
{
public:
	PoisonTower() = default;
	PoisonTower(Ref<Scene> scene);

	void OnUpdate(float dt) override;
	void AddTrackingEnemy(Enemy* target) override;
	void RemoveTrackingEnemy(Enemy* target) override;

protected:
	void changeModel(Ref<Scene> scene);

	std::vector<Enemy*> m_TargetList;
	std::unordered_map<Entity, MageBallData> m_AttackList;

	float m_FireInterval = 0.7f;
	float m_MageBallInitailSpeed = 80.0f;
	float m_MageBallTrackingSpeed = 200.0f;
	float m_MageBallAcceleration = 30.0f;
	float m_Timer = 0.0f;
	const static glm::vec3 s_AttackPointOffset;
};