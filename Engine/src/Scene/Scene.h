#pragma once

#include "entt/entt.hpp"
#include "Core/Base.h"
#include "SceneRenderer.h"



class Entity;

class Scene
{
public:
	Scene(const std::string& name = "Scene");
	~Scene() {}

	void OnUpdate(float dt);
	

	Entity CreateEntity(const std::string& name = "");
	Entity CreateEntity(Entity entity, const std::string& name = "");
	void DestroyEntity(Entity entity);

	template<typename... Components>
	auto GetAllEntitiesWith() { return m_Registry.view<Components...>(); }

	entt::registry& GetRegistry() { return m_Registry; }

	const std::string& GetName() const { return m_Name; }
	void SetName(const std::string& name) { m_Name = name; }

	static Ref<Scene> Copy(Ref<Scene> source);

private:
	entt::registry m_Registry;
	Ref<SceneRenderer> m_Renderer;
	std::string m_Name = "Untitled";

	friend class Entity;
	friend class SceneSerializer;
};

