#pragma once
#include "CometPCH.h"

#include "entt.hpp"

namespace Comet
{
	class Entity;

	class Scene
	{
	public:
		Scene() = default;
		static Reference<Scene> create() { return createReference<Scene>(); }

		Entity createEntity(const std::string& tag = "");

		void onViewportResized(uint32_t width, uint32_t height);

	private:
		template<typename T>
		void onComponentConstruction(Entity entity, T& component);

	private:
		entt::registry m_registry;

		uint32_t m_viewportWidth = 0, m_viewportHeight = 0;

		friend class Entity;
	};

}