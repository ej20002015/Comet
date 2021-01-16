#pragma once
#include "CometPCH.h"

#include "Scene.h"

#include "entt.hpp"

namespace Comet
{

	class Entity
	{
	public:
		Entity() = default;
		Entity(Scene* scene, entt::entity entityHandle)
			: m_scene(scene), m_entityHandle(entityHandle) {}

		Entity(const Entity& other) = default;
		~Entity() = default;

		template<typename T, typename ...Args>
		T& addComponent(Args&&... args)
		{
			CMT_CLIENT_ASSERT_MESSAGE(!hasComponent<T>(), "Cannot add component - entity already has component");
			return m_scene->m_registry.emplace<T>(m_entityHandle, std::forward<Args>(args)...);
		}

		template<typename T>
		void removeComponent()
		{
			CMT_CLIENT_ASSERT_MESSAGE(hasComponent<T>(), "Cannot remove component - entity does not have component");
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
			CMT_CLIENT_ASSERT_MESSAGE(hasComponent<T>(), "Cannot get component - entity does not have component");
			return m_scene->m_registry.get<T>(m_entityHandle);
		}

		operator bool() { return (m_entityHandle != entt::null) && m_scene; }

		bool operator ==(const Entity& other) const { return m_entityHandle == other.m_entityHandle; }

		bool operator !=(const Entity& other) const { return !(*this == other); }

	private:
		Scene* m_scene = nullptr;
		entt::entity m_entityHandle = entt::null;
	};

}