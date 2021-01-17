#include "CometPCH.h"
#include "Scene.h"

#include "Entity.h"
#include "Components.h"

namespace Comet
{

	Entity Scene::createEntity(const std::string& tag)
	{
		Entity newEntity(this, m_registry.create());

		//All entities will have a UUID and a transform
		newEntity.addComponent<UUIDComponent>();
		newEntity.addComponent<TransformComponent>();

		if (!tag.empty())
			newEntity.addComponent<TagComponent>(tag);

		return newEntity;
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

	template<typename T>
	void Scene::onComponentConstruction(Entity entity, T& component)
	{
	}

	template<>
	void Scene::onComponentConstruction(Entity entity, CameraComponent& cameraComponent)
	{
		if (m_viewportWidth != 0 && m_viewportHeight != 0)
			cameraComponent.camera.setViewportSize(m_viewportWidth, m_viewportHeight);
	}

}
