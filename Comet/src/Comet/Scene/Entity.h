#pragma once
#include "CometPCH.h"

#include "Scene.h"
#include "ComponentUtils.h"

#include "entt.hpp"

namespace Comet
{

class Entity
{
public:
	using Handle = entt::entity;
	
	Entity() = default;


	Entity(Scene* const scene, const Handle entityHandle)
		: m_scene(scene), m_entityHandle(entityHandle) {}

	Entity(const Entity& other) = default;
	~Entity() = default;

	template<typename T, typename ...Args>
	T& addComponent(Args&&... args)
	{
		if (hasComponent<T>())
			throw CometException() << "Cannot add '" << ComponentUtils::getComponentName<T>() << "' component - entity already has component";

		auto& component = m_scene->m_registry.emplace<T>(m_entityHandle, std::forward<Args>(args)...);
		m_scene->onComponentConstruction<T>(*this);
		return component;
	}

	template<typename T>
	void removeComponent()
	{
		if (!hasComponent<T>())
			throw CometException() << "Cannot remove '" << ComponentUtils::getComponentName<T>() << "' component - entity does not have component";

		return m_scene->m_registry.remove<T>(m_entityHandle);
	}

	template<typename T>
	bool hasComponent()
	{
		return m_scene->m_registry.has<T>(m_entityHandle);
	}

	template<typename T>
	T& getComponent()
	{
		if (!hasComponent<T>())
			throw CometException() << "Cannot get '" << ComponentUtils::getComponentName<T>() << "' component - entity does not have component";

		return m_scene->m_registry.get<T>(m_entityHandle);
	}

	operator bool() { return (m_entityHandle != entt::null) && m_scene; }

	bool operator ==(const Entity& other) const { return m_entityHandle == other.m_entityHandle; }

	bool operator !=(const Entity& other) const { return !(*this == other); }

private:
	Scene* m_scene = nullptr;
	entt::entity m_entityHandle = entt::null;

	friend class Scene;
};

}