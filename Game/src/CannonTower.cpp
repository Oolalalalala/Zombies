#include "CannonTower.h"

const glm::vec3 CannonTower::s_AttackPointOffset = glm::vec3(0.0f, 160.0f, 0.0f);

CannonTower::CannonTower(Ref<Scene> scene)
	:Tower(scene, 100.0, 100.0, Asset::CannonTowerLevel1Model)
{
}
CannonTower::~CannonTower()
{
	for (auto it : m_AttackList)
	{
		AssetLibrary::DestoryModel(it.first);
	}
}
void CannonTower::OnUpdate(float dt)
{
	if (m_TargetList.size())
	{
		m_Timer += dt;

		if (m_Timer > m_FireInterval) // Spawn an arrow
		{
			m_Timer -= m_FireInterval;
			Enemy* target = m_TargetList[0];

			Entity cannonBall = AssetLibrary::GetModel(Asset::CannonBallModel);
			auto& transform = cannonBall.GetComponent<TransformComponent>();
			transform.Position = _entity.GetComponent<TransformComponent>().Position + s_AttackPointOffset;
			cannonBall.GetComponent<MeshRendererComponent>().Enabled = true;
			m_AttackList[cannonBall].Target = target;
		}
	}
	else
		m_Timer = m_FireInterval;

	for (auto it = m_AttackList.begin(); it != m_AttackList.end();)
	{
		Entity cannonBall = it->first;
		Enemy* target = it->second.Target;

		auto& transform = cannonBall.GetComponent<TransformComponent>();
		glm::vec3& arrowPos = transform.Position;
		glm::vec3 targetPos = target ? target->getPosition() : it->second.FallbackPosition;

		glm::vec3 delta = targetPos - arrowPos;
		glm::vec3 dir = glm::normalize(delta);

		glm::vec3 dx = dir * m_CannonBallSpeed * dt;

		arrowPos += dx;

		if (glm::length2(delta) < m_CannonBallSpeed * m_CannonBallSpeed * dt * dt)
		{
			if (target)
				target->takeDamage(_damage);
			AssetLibrary::DestoryModel(cannonBall);
			it = m_AttackList.erase(it);
		}
		else
			it++;
	}
}

void CannonTower::AddTrackingEnemy(Enemy* target)
{
	m_TargetList.push_back(target);
}

void CannonTower::RemoveTrackingEnemy(Enemy* target)
{
	for (auto it = m_AttackList.begin(); it != m_AttackList.end();)
	{
		if (it->second.Target == target)
		{
			it->second.FallbackPosition = it->second.Target->getPosition();
			it->second.Target = nullptr;
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
void CannonTower::changeModel(Ref<Scene> scene)
{
	TransformComponent oldTransform = _entity.GetComponent<TransformComponent>();

	switch (_level)
	{
	case 1:
		Destroy(scene);
		_entity = AssetLibrary::GetModel(Asset::CannonTowerLevel1Model);
		m_FireInterval = 1.2f;
		break;
	case 2:
		Destroy(scene);
		_entity = AssetLibrary::GetModel(Asset::CannonTowerLevel2Model);
		m_FireInterval = 1.0f;
		break;
	case 3:
		Destroy(scene);
		_entity = AssetLibrary::GetModel(Asset::CannonTowerLevel3Model);
		m_FireInterval = 0.8f;
		break;
	case 4:
		Destroy(scene);
		_entity = AssetLibrary::GetModel(Asset::CannonTowerLevel4Model);
		m_FireInterval = 0.6f;
		break;
	}
	_entity.GetComponent<TransformComponent>() = oldTransform;
}