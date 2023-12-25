#include "PoisonTower.h"

#include "glm/gtc/random.hpp"

const glm::vec3 PoisonTower::s_AttackPointOffset = glm::vec3(0.0f, 160.0f, 0.0f);

PoisonTower::PoisonTower(Ref<Scene> scene)
	:Tower(scene, 100.0, 100.0, Asset::PoisonTowerLevel1Model)
{
}

PoisonTower::~PoisonTower()
{
	for (auto it : m_AttackList)
	{
		AssetLibrary::DestoryModel(it.first);
	}
}

void PoisonTower::OnUpdate(float dt)
{
	if (m_TargetList.size())
	{
		m_Timer += dt;

		if (m_Timer > m_FireInterval) // Spawn an arrow
		{
			m_Timer -= m_FireInterval;
			Enemy* target = m_TargetList[0];

			Entity mageBall = AssetLibrary::GetModel(Asset::CannonBallModel);
			auto& transform = mageBall.GetComponent<TransformComponent>();
			transform.Position = _entity.GetComponent<TransformComponent>().Position + s_AttackPointOffset;
			mageBall.GetComponent<MeshRendererComponent>().Enabled = true;

			glm::vec3 initialVelocity = glm::ballRand(1.0f);
			initialVelocity.y = glm::abs(initialVelocity.y);
			initialVelocity = glm::normalize(initialVelocity) * m_MageBallInitailSpeed;
			m_AttackList[mageBall] = { target, initialVelocity, 0, glm::vec3(0.0f) };
		}
	}
	else
		m_Timer = m_FireInterval;

	for (auto it = m_AttackList.begin(); it != m_AttackList.end();)
	{
		Entity mageBall = it->first;
		auto& [target, velocity, airTime, fallbackPos] = it->second;
		airTime += dt;

		auto& transform = mageBall.GetComponent<TransformComponent>();
		glm::vec3& ballPos = transform.Position;
		glm::vec3 targetPos = target ? target->getPosition() : fallbackPos;

		glm::vec3 delta = targetPos - ballPos;
		glm::vec3 dir = glm::normalize(delta);
		if (airTime >= 2.5f)
		{
			velocity = glm::mix(dir * m_MageBallTrackingSpeed, velocity, exp((2.5 - airTime) * 0.03));
		}
		else
			velocity += dir * m_MageBallAcceleration * dt;
		glm::vec3 dx = velocity * dt;


		ballPos += dx;

		if (glm::length2(delta) < glm::length2(velocity) * dt)
		{
			if (target)
				target->takeDamage(_damage);
			AssetLibrary::DestoryModel(mageBall);
			it = m_AttackList.erase(it);
		}
		else
			it++;
	}
}

void PoisonTower::AddTrackingEnemy(Enemy* target)
{
	m_TargetList.push_back(target);
}

void PoisonTower::RemoveTrackingEnemy(Enemy* target)
{
	for (auto it = m_AttackList.begin(); it != m_AttackList.end();)
	{
		if (it->second.Enemy == target)
		{
			it->second.FallbackPosition = it->second.Enemy->getPosition();
			it->second.Enemy = nullptr;
		}
		it++;
	}

	for (auto it = m_TargetList.begin(); it != m_TargetList.end(); it++)
	{
		if (*it == target)
		{
			m_TargetList.erase(it);
			break;
		}
	}
}
void PoisonTower::changeModel(Ref<Scene> scene)
{
	TransformComponent oldTransform = _entity.GetComponent<TransformComponent>();

	switch (_level)
	{
	case 1:
		Destroy(scene);
		_entity = AssetLibrary::GetModel(Asset::PoisonTowerLevel1Model);
		m_FireInterval = 0.6f;
		break;
	case 2:
		Destroy(scene);
		_entity = AssetLibrary::GetModel(Asset::PoisonTowerLevel2Model);
		m_FireInterval = 0.47f;
		break;
	case 3:
		Destroy(scene);
		_entity = AssetLibrary::GetModel(Asset::PoisonTowerLevel3Model);
		m_FireInterval = 0.35f;
		break;
	case 4:
		Destroy(scene);
		_entity = AssetLibrary::GetModel(Asset::PoisonTowerLevel4Model);
		m_FireInterval = 0.2f;
		break;
	}
	_entity.GetComponent<TransformComponent>() = oldTransform;
}