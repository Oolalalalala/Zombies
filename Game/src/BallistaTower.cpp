#include "BallistaTower.h"

#include "glm/gtx/rotate_vector.hpp"


const glm::vec3 BallistaTower::s_AttackPointOffset = glm::vec3(0.0f, 150.0f, 0.0f);

BallistaTower::BallistaTower(Ref<Scene> scene)
	:Tower(scene, 100.0, 100.0, Asset::BallistaTowerLevel1Model)
{
}
BallistaTower::~BallistaTower()
{
	for (auto it : m_AttackList)
	{
		AssetLibrary::DestoryModel(it.first);
	}
}

void BallistaTower::OnUpdate(float dt)
{
	if (m_TargetList.size())
	{
		m_Timer += dt;

		if (m_Timer > m_FireInterval) // Spawn an arrow
		{
			m_Timer -= m_FireInterval;
			Enemy* target = m_TargetList[0];

			Entity arrow = AssetLibrary::GetModel(Asset::ArrowModel);
			auto& transform = arrow.GetComponent<TransformComponent>();
			transform.Position = _entity.GetComponent<TransformComponent>().Position + s_AttackPointOffset;
			transform.Scale *= 0.5f;
			arrow.GetComponent<MeshRendererComponent>().Enabled = true;
			m_AttackList[arrow].Target = target;
		}
	}
	else
		m_Timer = m_FireInterval;

	for (auto it = m_AttackList.begin(); it != m_AttackList.end();)
	{
		Entity arrow = it->first;
		Enemy* target = it->second.Target;


		auto& transform = arrow.GetComponent<TransformComponent>();
		glm::vec3& arrowPos = transform.Position;
		glm::vec3 targetPos = target ? target->getPosition() : it->second.FallbackPosition;

		glm::vec3 delta = targetPos - arrowPos;
		glm::vec3 dir = glm::normalize(delta);
		glm::vec3 rot = glm::rotate(dir, glm::radians(90.0f), glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), dir));
		transform.Rotation = glm::quatLookAt(rot, glm::vec3(0.0f, -1.0f, 0.0f));

		glm::vec3 dx = dir * m_ArrowSpeed * dt;

		arrowPos += dx;

		if (glm::length2(delta) < m_ArrowSpeed * m_ArrowSpeed * dt * dt)
		{
			if (target)
				target->takeDamage(_damage * 0.5f);

			AssetLibrary::DestoryModel(arrow);
			it = m_AttackList.erase(it);
		}
		else
			it++;
	}
}

void BallistaTower::AddTrackingEnemy(Enemy* target)
{
	m_TargetList.push_back(target);
}

void BallistaTower::RemoveTrackingEnemy(Enemy* target)
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
void BallistaTower::changeModel(Ref<Scene> scene)
{
	TransformComponent oldTransform = _entity.GetComponent<TransformComponent>();

	switch (_level)
	{
	case 1:
		Destroy(scene);
		_entity = AssetLibrary::GetModel(Asset::BallistaTowerLevel1Model);
		m_FireInterval = 0.15f;
		m_ArrowSpeed = 500.0f;
		break;
	case 2:
		Destroy(scene);
		_entity = AssetLibrary::GetModel(Asset::BallistaTowerLevel2Model);
		m_FireInterval = 0.10f;
		m_ArrowSpeed = 800.0f;
		break;
	case 3:
		Destroy(scene);
		_entity = AssetLibrary::GetModel(Asset::BallistaTowerLevel3Model);
		m_FireInterval = 0.07f;
		m_ArrowSpeed = 1200.0f;
		break;
	case 4:
		Destroy(scene);
		_entity = AssetLibrary::GetModel(Asset::BallistaTowerLevel4Model);
		m_FireInterval = 0.05f;
		m_ArrowSpeed = 1500.0f;
		break;
	}
	_entity.GetComponent<TransformComponent>() = oldTransform;
}