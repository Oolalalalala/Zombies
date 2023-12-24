#include "CannonTower.h"

const glm::vec3 CannonTower::s_AttackPointOffset = glm::vec3(0.0f, 160.0f, 0.0f);

CannonTower::CannonTower(Ref<Scene> scene)
	:Tower(scene, 100.0, 100.0, Asset::CannonTowerLevel1Model)
{
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
			m_AttackList[cannonBall] = target;
		}
	}
	else
		m_Timer = m_FireInterval;

	for (auto it = m_AttackList.begin(); it != m_AttackList.end();)
	{
		Entity cannonBall = it->first;
		Enemy* target = it->second;

		auto& transform = cannonBall.GetComponent<TransformComponent>();
		glm::vec3& arrowPos = transform.Position;
		glm::vec3 targetPos = target->getPosition();

		glm::vec3 delta = targetPos - arrowPos;
		glm::vec3 dir = glm::normalize(delta);

		glm::vec3 dx = dir * m_CannonBallSpeed * dt;

		arrowPos += dx;

		if (glm::length2(delta) < 4 * m_CannonBallSpeed * m_CannonBallSpeed * dt * dt)
		{
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
		if (it->second == target)
		{
			AssetLibrary::DestoryModel(it->first);
			it = m_AttackList.erase(it);
		}
		else
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
		break;
	case 2:
		Destroy(scene);
		_entity = AssetLibrary::GetModel(Asset::CannonTowerLevel2Model);
		break;
	case 3:
		Destroy(scene);
		_entity = AssetLibrary::GetModel(Asset::CannonTowerLevel3Model);
		break;
	case 4:
		Destroy(scene);
		_entity = AssetLibrary::GetModel(Asset::CannonTowerLevel4Model);
		break;
	}
	_entity.GetComponent<TransformComponent>() = oldTransform;
}