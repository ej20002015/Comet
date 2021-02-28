#include "CometPCH.h"
#include "Scene.h"

#include "Entity.h"
#include "Components.h"

#include "Comet/Renderer/Renderer2D.h"

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

	void Scene::deleteEntity(Entity entity)
	{
		m_registry.destroy(entity.m_entityHandle);
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

		Entity primaryCameraEntity;

		//Get primary camera
		{
			auto view = m_registry.view<CameraComponent>();
			for (auto entity : view)
			{
				CameraComponent& cameraComponent = view.get<CameraComponent>(entity);
				if (cameraComponent.primary)
				{
					primaryCameraEntity = { this, entity };
					break;
				}
			}
		}

		if (!primaryCameraEntity)
		{
			Log::cometError("Cannot render scene - no primary camera set");
			//Only works if rendering commands are the last ones in update
			return;
		}

		//Begin scene
		const Camera& camera = primaryCameraEntity.getComponent<CameraComponent>().camera;
		const glm::mat4& cameraTransform = primaryCameraEntity.getComponent<TransformComponent>().getTransform();
		//Render with no depth testing for 2D scene
		Renderer2D::beginScene(camera, cameraTransform, false);

		//Render Sprites
		{
			//Partial owning group as transform will be used in other views/groups
			auto group = m_registry.group<SpriteComponent>(entt::get<TransformComponent>);
			//Sort by transform z coordinate translation so sprites are drawn in the correct order
			group.sort<TransformComponent>([](const TransformComponent& lhs, const TransformComponent& rhs)
			{
					return lhs.translation.z < rhs.translation.z;
			});

			for (auto entity : group)
			{
				const SpriteComponent& spriteComponent = group.get<SpriteComponent>(entity);
				const glm::mat4& transform = group.get<TransformComponent>(entity).getTransform();

				//Depending on spriteTextureType make the correct draw command to the renderer
				if (spriteComponent.spriteTextureType == SpriteComponent::SpriteTextureType::NORMAL)
					Renderer2D::drawQuad(transform, spriteComponent.color, spriteComponent.texture, spriteComponent.tilingFactor);
				else
				{
					Renderer2D::drawSubQuad(transform, spriteComponent.color, spriteComponent.subTexture, spriteComponent.tilingFactor);
				}
			}
		}

		Renderer2D::endScene();
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
