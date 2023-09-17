#pragma once
#include "CometPCH.h"

#include "entt.hpp"

#include "glm/glm.hpp"

#include "Comet/Core/Dispatcher.h"
#include "Comet/Renderer/EditorCamera.h"
#include "Comet/Renderer/SceneRenderer.h"

namespace Comet
{
class Entity;
class SceneHierarchyPanel;
struct CameraComponent;
struct TagComponent;
struct PointLightComponent;
class SceneSerializer;

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
			T& component = entity.template getComponent<T>();
			function(entity, component); 
		}
	};

	Scene(const std::string_view sceneName = "Unnamed Scene");

	static Reference<Scene> create() { return createReference<Scene>(); }

	Entity createEntity(const std::string_view tagString = "Unnamed Entity");
	void deleteEntity(const Entity entity);

	void onEditorUpdate(const Timestep ts, const float exposure, const Reference<Framebuffer>& targetFrambuffer, const EditorCamera& editorCamera);
	void onRuntimeStart();
	void onRuntimeUpdate(const Timestep ts, const float exposure, const Reference<Framebuffer>& targetFrambuffer);
	void onRuntimeStop();

	void onViewportResized(const uint32_t width, const uint32_t height);

	template<typename T>
	void bindOnComponentConstructionFunction(const std::function<void(Entity, T&)>& function)
	{
		s_constructCallbacks[typeid(T)].push_back(createUnique<ComponentCallbackDispatcher<T>>(function));
	}

	Entity getPrimaryCameraEntity();

	const std::string_view getSceneName() const { return m_sceneName; }
	void setSceneName(const std::string_view sceneName) { m_sceneName = sceneName; }

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

	void renderModels();
	void renderSprites();

	SceneRenderer::PointLightList getPointLights();

private:
	entt::registry m_registry;

	uint32_t m_viewportWidth = 0, m_viewportHeight = 0;
	std::string m_sceneName;

	static std::unordered_map<std::type_index, std::vector<Unique<Dispatcher>>> s_constructCallbacks;

	friend class Entity;
	friend class SceneHierarchyPanel;
	friend class SceneSerializer;
};

}