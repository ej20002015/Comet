#include "CometPCH.h"
#include "Scene.h"

#include "Entity.h"
#include "Components.h"

#include "Comet/Renderer/Renderer2D.h"

namespace Comet
{

std::unordered_map<std::type_index, std::vector<Unique<Dispatcher>>> Scene::s_constructCallbacks;

Scene::Scene(const std::string_view sceneName)
	: m_sceneName(sceneName)
{
	bindOnComponentConstructionFunction<CameraComponent>(CMT_BIND_METHOD(Scene::onCameraComponentConstruction));
	bindOnComponentConstructionFunction<TagComponent>(CMT_BIND_METHOD(Scene::onTagComponentCreation));
}

Entity Scene::createEntity(const std::string_view tagString)
{
	Entity newEntity(this, m_registry.create());

	//All entities will have a UUID and a transform
	newEntity.addComponent<UUIDComponent>();
	newEntity.addComponent<TransformComponent>();

	newEntity.addComponent<TagComponent>(tagString);

	return newEntity;
}

void Scene::deleteEntity(const Entity entity)
{
	m_registry.destroy(entity.m_entityHandle);
}

void Scene::onEditorUpdate(const Timestep ts, const EditorCamera& editorCamera)
{
	//Render with no depth testing for 2D scene
	Renderer2D::beginScene(editorCamera, true);
	renderSprites();
	Renderer2D::endScene();
}

void Scene::renderSprites()
{
	//Partial owning group as transform will be used in other views/groups
	auto group = m_registry.group<SpriteComponent>(entt::get<TransformComponent>);

	//Sort by transform z coordinate translation so sprites are drawn in the correct order
	group.sort<TransformComponent>([](const TransformComponent& lhs, const TransformComponent& rhs)
	{
		return lhs.translation.z < rhs.translation.z;
	});

	for (entt::entity entity : group)
	{
		const SpriteComponent& spriteComponent = group.get<SpriteComponent>(entity);
		const glm::mat4& transform = group.get<TransformComponent>(entity).getTransform();

		if (spriteComponent.spriteTextureType == SpriteComponent::TextureType::NORMAL)
			Renderer2D::drawQuad(transform, spriteComponent.color, spriteComponent.texture, spriteComponent.tilingFactor, static_cast<int32_t>(entity));
		else
			Renderer2D::drawSubQuad(transform, spriteComponent.color, spriteComponent.subTexture, spriteComponent.tilingFactor, static_cast<int32_t>(entity));
	}
}

void Scene::onRuntimeUpdate(const Timestep ts)
{
	//Update scripts
	{
		auto view = m_registry.view<NativeScriptComponent>();
		for (entt::entity entity : view)
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

	Entity primaryCameraEntity = getPrimaryCameraEntity();

	if (!primaryCameraEntity)
	{
		Log::cometError("Cannot render scene - no primary camera set");
		return;
	}

	const Camera& camera = primaryCameraEntity.getComponent<CameraComponent>().camera;
	const glm::mat4& cameraTransform = primaryCameraEntity.getComponent<TransformComponent>().getTransform();
	//Render with no depth testing for 2D scene
	Renderer2D::beginScene(camera, cameraTransform, false);
	renderSprites();
	Renderer2D::endScene();
}

void Scene::onViewportResized(const uint32_t width, const uint32_t height)
{
	m_viewportWidth = width;
	m_viewportHeight = height;

	//Set viewport for all cameras in the scene that have a non-fixed aspect ratio
	auto view = m_registry.view<CameraComponent>();
	for (entt::entity entity : view)
	{
		auto& camera = view.get<CameraComponent>(entity).camera;
		if (!camera.getFixedAspectRatio())
		{
			camera.setViewportSize(width, height);
		}
	}
}

Entity Scene::getPrimaryCameraEntity()
{
	Entity primaryCameraEntity;

	auto view = m_registry.view<CameraComponent>();
	for (const entt::entity entity : view)
	{
		CameraComponent& cameraComponent = view.get<CameraComponent>(entity);
		if (cameraComponent.primary)
		{
			primaryCameraEntity = { this, entity };
			break;
		}
	}

	return primaryCameraEntity;
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
