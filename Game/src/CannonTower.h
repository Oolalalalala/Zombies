#pragma once
#include "Tower.h"
class CannonTower :public Tower
{
public:
	CannonTower() = default;
	CannonTower(Ref<Scene> scene);

	void OnUpdate(float dt) override;
	void AddToTargetList(Enemy* target) override;
	void RemoveFromTargetList(Enemy* target) override;

protected:
	void changeModel(Ref<Scene> scene);

	std::vector<Enemy*> m_TargetList;
	std::unordered_map<Entity, Enemy*> m_AttackList;

	float m_FireInterval = 0.7f;
	float m_ArrowSpeed = 300.0f;
	float m_Timer = 0.0f;
	const static glm::vec3 s_AttackPointOffset;
};