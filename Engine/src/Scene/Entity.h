#pragma once

#include "entt/entt.hpp"
#include "Scene.h"

	
class Entity
{
public:
	Entity() = default;
	Entity(entt::entity entityID, Scene* scene)
		: m_EntityID(entityID), m_Scene(scene) {}

	template<typename T, typename ... Args>
	T& AddComponent(Args&& ... args)
	{
		return m_Scene->m_Registry.emplace<T>(m_EntityID, std::forward<Args>(args)...);
	}

	template<typename ... Args>
	void RemoveComponent()
	{
		m_Scene->m_Registry.remove_if_exists<Args...>(m_EntityID);
	}

	template<typename ... Args>
	decltype(auto) GetComponent()
	{
		return m_Scene->m_Registry.get<Args...>(m_EntityID);
	}

	template<typename ... Args>
	bool HasComponent()
	{
		return m_Scene->m_Registry.has<Args...>(m_EntityID);
	}

	template<typename ... Args>
	bool HasAnyComponent()
	{
		return m_Scene->m_Registry.any<Args...>(m_EntityID);
	}

	operator bool() const { return m_EntityID != entt::null; }

	bool operator==(const Entity& other) const
	{
		return m_EntityID == other.m_EntityID && m_Scene == other.m_Scene;
	}

	bool operator!=(const Entity& other) const
	{
		return !(*this == other);
	}
private:
	entt::entity m_EntityID = entt::null;
	Scene* m_Scene = nullptr;

	friend class Scene;
	friend class std::hash<Entity>;
};



namespace std {

	template<>
	struct hash<Entity>
	{
		inline std::size_t operator()(const Entity& entity) const
		{
			return hash<uint64_t>()((uint32_t)entity.m_EntityID & (uint64_t)entity.m_Scene);
		}
	};

}
