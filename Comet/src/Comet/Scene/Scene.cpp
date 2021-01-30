#include "CometPCH.h"
#include "Scene.h"

#include "Entity.h"
#include "Components.h"

namespace Comet
{

	std::unordered_map<std::type_index, std::vector<Unique<Dispatcher>>> Scene::s_constructCallbacks;

	Scene::Scene()
	{
		bindOnComponentConstructionFunction<CameraComponent>(CMT_BIND_METHOD(Scene::onCameraComponentConstruction));
		bindOnComponentConstructionFunction<TagComponent>(CMT_BIND_METHOD(Scene::onTagComponentCreation));
	}

	Entity Scene::createEntity(const std::string& tag)
	{
		Entity newEntity(this, m_registry.create());

		//All entities will have a UUID and a transform
		newEntity.addComponent<UUIDComponent>();
		newEntity.addComponent<TransformComponent>();

		if (!tag.empty())
			newEntity.addComponent<TagComponent>(tag);
		else
			newEntity.addComponent<TagComponent>();

		return newEntity;
	}

	void Scene::onUpdate(Timestep ts)
	{
		//Update scripts
		{
			auto view = m_registry.view<NativeScriptComponent>();
			for (auto entity : view)
			{
				NativeScriptComponent& nativeScriptComponent = view.get<NativeScriptComponent>(entity);
				if (!nativeScriptComponent.script)
				{
					nativeScriptComponent.script = nativeScriptComponent.constructScript();
					if (nativeScriptComponent.script)
					{	
						nativeScriptComponent.script->m_entity = Entity{ this, entity };
						nativeScriptComponent.script->onStart();
					}
				}

				nativeScriptComponent.script->onUpdate(ts);
			}
		}

		//TODO: render commands in here
	}

	void Scene::onViewportResized(uint32_t width, uint32_t height)
	{
		m_viewportWidth = width;
		m_viewportHeight = height;

		//Set viewport for all cameras in the scene that have a non-fixed aspect ratio
		auto view = m_registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& camera = view.get<CameraComponent>(entity).camera;
			if (!camera.getFixedAspectRatio())
			{
				camera.setViewportSize(width, height);
			}
		}
	}

	void Scene::onCameraComponentConstruction(Entity entity, CameraComponent& cameraComponent)
	{
		if (m_viewportWidth != 0 && m_viewportHeight != 0)
			cameraComponent.camera.setViewportSize(m_viewportWidth, m_viewportHeight);
	}

	void Scene::onTagComponentCreation(Entity entity, TagComponent& tagComponent)
	{
		UUIDComponent uuidComponent = entity.getComponent<UUIDComponent>();
		Log::cometInfo("Creating entity with UUID = {0} and tag = '{1}'", static_cast<std::string>(uuidComponent), static_cast<std::string>(tagComponent));
	}

}
