#pragma once
#include "Tower.h"
#include <deque>

struct LightBeamData
{
	Entity LightBeam;
	std::vector<Entity> Particles;
};


class WizardTower :public Tower
{
public:
	WizardTower() = default;
	WizardTower(Ref<Scene> scene);

	void OnUpdate(float dt) override;
	void AddTrackingEnemy(Enemy* target) override;
	void RemoveTrackingEnemy(Enemy* target) override;

protected:
	void changeModel(Ref<Scene> scene);

	std::deque<Enemy*> m_TargetList;
	std::unordered_map<Enemy*, LightBeamData> m_AttackList;

	float m_CannonBallSpeed = 300.0f;
	uint32_t m_MaxConcurrentTarget = 3;
	const static glm::vec3 s_AttackPointOffset;
};