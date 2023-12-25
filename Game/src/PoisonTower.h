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
	~PoisonTower();

	void OnUpdate(float dt) override;
	void AddTrackingEnemy(Enemy* target) override;
	void RemoveTrackingEnemy(Enemy* target) override;

protected:
	void changeModel(Ref<Scene> scene);

	std::vector<Enemy*> m_TargetList;
	std::unordered_map<Entity, MageBallData> m_AttackList;

	float m_FireInterval = 0.6f;
	float m_MageBallInitailSpeed = 100.0f;
	float m_MageBallTrackingSpeed = 500.0f;
	float m_MageBallAcceleration = 50.0f;
	float m_Timer = 0.0f;
	const static glm::vec3 s_AttackPointOffset;
};