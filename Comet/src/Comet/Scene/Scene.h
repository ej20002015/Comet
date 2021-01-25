#pragma once
#include "CometPCH.h"

#include "entt.hpp"

#include <any>

namespace Comet
{
	class Entity;

	class Scene
	{
	public:

		Scene();
		static Reference<Scene> create() { return createReference<Scene>(); }

		Entity createEntity(const std::string& tag = "");

		void onViewportResized(uint32_t width, uint32_t height);

		using ComponentCallbackFunction = std::function<void(Entity)>;

		template<typename T>
		void bindOnComponentConstructionFunction(const ComponentCallbackFunction& function)
		{
			s_constructCallbacks[typeid(T)] = function;
		}

	private:
		template<typename T>
		void onComponentConstruction(Entity entity)
		{
			if (auto it = s_constructCallbacks.find(typeid(T)); it != s_constructCallbacks.end())
			{
				ComponentCallbackFunction function = it->second;
				function(entity);
			}
		}

		void onCameraComponentConstruction(Entity entity);

	private:
		entt::registry m_registry;

		uint32_t m_viewportWidth = 0, m_viewportHeight = 0;

		static std::unordered_map<std::type_index, ComponentCallbackFunction> s_constructCallbacks;

		friend class Entity;
	};

}