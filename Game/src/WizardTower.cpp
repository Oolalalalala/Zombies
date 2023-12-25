#include "WizardTower.h"

const glm::vec3 WizardTower::s_AttackPointOffset = glm::vec3(0.0f, 193.0f, 0.0f);

WizardTower::WizardTower(Ref<Scene> scene)
	:Tower(scene, 1.0, 100.0, Asset::WizardTowerLevel1Model)
{
}
WizardTower::~WizardTower()
{
	for (auto it : m_AttackList)
	{
		AssetLibrary::DestoryModel(it.second.LightBeam);
	}
}
void WizardTower::OnUpdate(float dt)
{
	if (m_TargetList.size() && m_AttackList.size() < m_MaxConcurrentTarget)
	{
		Enemy* target = m_TargetList.front();
		m_TargetList.pop_front();

		Entity lightBeam = AssetLibrary::GetModel(Asset::LightBeamModel);
		auto& transform = lightBeam.GetComponent<TransformComponent>();
		transform.Position = _entity.GetComponent<TransformComponent>().Position + s_AttackPointOffset;
		lightBeam.GetComponent<MeshRendererComponent>().Enabled = true;
		m_AttackList[target].LightBeam = lightBeam;
	}


	for (auto it = m_AttackList.begin(); it != m_AttackList.end();)
	{
		Enemy* target = it->first;
		auto& [lightBeam, particles] = it->second;

		glm::vec3 sourcePos = _entity.GetComponent<TransformComponent>().Position + s_AttackPointOffset;
		glm::vec3 targetPos = target->getPosition();
		glm::vec3 midPointPos = (sourcePos + targetPos) * 0.5f;

		glm::vec3 delta = targetPos - sourcePos;
		glm::vec3 dir = glm::normalize(delta);

		auto& transform = lightBeam.GetComponent<TransformComponent>();
		transform.Position = midPointPos;
		transform.Scale.y = glm::length(delta) * 0.5f;
		transform.Rotation = glm::quat(glm::vec3(0.0f, 1.0f, 0.0f), dir);

		target->takeDamage(_damage * dt / glm::pow(m_AttackList.size(), 0.3));

		it++;
	}
}

void WizardTower::AddTrackingEnemy(Enemy* target)
{
	m_TargetList.push_back(target);
}

void WizardTower::RemoveTrackingEnemy(Enemy* target)
{
	for (auto it = m_AttackList.begin(); it != m_AttackList.end();)
	{
		if (it->first == target)
		{
			AssetLibrary::DestoryModel(it->second.LightBeam);
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
void WizardTower::changeModel(Ref<Scene> scene)
{
	TransformComponent oldTransform = _entity.GetComponent<TransformComponent>();

	switch (_level)
	{
	case 1:
		Destroy(scene);
		_entity = AssetLibrary::GetModel(Asset::WizardTowerLevel1Model);
		m_MaxConcurrentTarget = 2;
		break;
	case 2:
		Destroy(scene);
		_entity = AssetLibrary::GetModel(Asset::WizardTowerLevel2Model);
		m_MaxConcurrentTarget = 3;
		break;
	case 3:
		Destroy(scene);
		_entity = AssetLibrary::GetModel(Asset::WizardTowerLevel3Model);
		m_MaxConcurrentTarget = 4;
		break;
	case 4:
		Destroy(scene);
		_entity = AssetLibrary::GetModel(Asset::WizardTowerLevel4Model);
		m_MaxConcurrentTarget = 5;
		break;
	}
	_entity.GetComponent<TransformComponent>() = oldTransform;
}