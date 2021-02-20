#pragma once
#include "CometPCH.h"

#include "entt.hpp"

#include "Comet/Core/Dispatcher.h"

namespace Comet
{
	class Entity;
	class SceneHierarchyPanel;
	struct CameraComponent;
	struct TagComponent;

	class Scene
	{
	public:

		template<typename T>
		struct ComponentCallbackDispatcher : public Dispatcher
		{
			Entity entity;
			std::function<void(Entity, T&)> function;

			ComponentCallbackDispatcher() = default;

			ComponentCallbackDispatcher(const std::function<void(Entity, T&)>& function)
				: function(function) {}

			void invoke() override 
			{ 
				T component = entity.getComponent<T>();
				function(entity, component); 
			}
		};

		Scene();
		static Reference<Scene> create() { return createReference<Scene>(); }

		Entity createEntity(const std::string& tag = "");
		void deleteEntity(Entity entity);

		void onUpdate(Timestep ts);

		void onViewportResized(uint32_t width, uint32_t height);

		template<typename T>
		void bindOnComponentConstructionFunction(const std::function<void(Entity, T&)>& function)
		{
			s_constructCallbacks[typeid(T)].push_back(createUnique<ComponentCallbackDispatcher<T>>(function));
		}

	private:
		template<typename T>
		void onComponentConstruction(Entity entity)
		{
			if (auto it = s_constructCallbacks.find(typeid(T)); it != s_constructCallbacks.end())
			{
				for (Unique<Dispatcher>& dispatcherPointer : it->second)
				{
					ComponentCallbackDispatcher<T>& dispatcher = *static_cast<ComponentCallbackDispatcher<T>*>(dispatcherPointer.get());
					dispatcher.entity = entity;
					dispatcher.invoke();
				}
			}
		}

		void onCameraComponentConstruction(Entity entity, CameraComponent& cameraComponent);
		void onTagComponentCreation(Entity entity, TagComponent& tagComponent);

	private:
		entt::registry m_registry;

		uint32_t m_viewportWidth = 0, m_viewportHeight = 0;

		static std::unordered_map<std::type_index, std::vector<Unique<Dispatcher>>> s_constructCallbacks;

		friend class Entity;
		friend class SceneHierarchyPanel;
	};

}