#include "CometPCH.h"
#include "Scene.h"

#include "Entity.h"
#include "Components.h"

#include "Comet/Renderer/SceneRenderer.h"
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

	// All entities will have a UUID, a transform and a tag
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
	const auto pointLights = getPointLights();
	SceneRenderer::beginScene(editorCamera, pointLights);
	renderModels();
	SceneRenderer::endScene();

	Renderer2D::beginScene(editorCamera, true); // Render with no depth testing for 2D scene
	renderSprites();
	Renderer2D::endScene();
}

void Scene::onRuntimeUpdate(const Timestep ts)
{
	// Update scripts

	const auto view = m_registry.view<NativeScriptComponent>();
	view.each([this, ts](const entt::entity entity, NativeScriptComponent& nativeScriptComponent)
	{
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
	});

	const Entity primaryCameraEntity = getPrimaryCameraEntity();
	if (!primaryCameraEntity)
	{
		Log::cometError("Cannot render scene - no primary camera set");
		return;
	}

	const Camera& camera = primaryCameraEntity.getComponent<CameraComponent>().camera;
	const glm::mat4& cameraTransform = primaryCameraEntity.getComponent<TransformComponent>().getTransform();

	const auto pointLights = getPointLights();
	SceneRenderer::beginScene(camera, cameraTransform, pointLights);
	renderModels();
	SceneRenderer::endScene();

	Renderer2D::beginScene(camera, cameraTransform, false); // Render with no depth testing for 2D scene
	renderSprites();
	Renderer2D::endScene();
}

void Scene::onViewportResized(const uint32_t width, const uint32_t height)
{
	m_viewportWidth = width;
	m_viewportHeight = height;

	// Set viewport for all cameras in the scene that have a non-fixed aspect ratio
	const auto view = m_registry.view<CameraComponent>();
	view.each([=](const auto entity, CameraComponent& cameraComponent)
	{
		if (!cameraComponent.camera.getFixedAspectRatio())
			cameraComponent.camera.setViewportSize(width, height);
	});
}

Entity Scene::getPrimaryCameraEntity()
{
	Entity primaryCameraEntity;

	const auto view = m_registry.view<CameraComponent>();
	view.each([&, this](const auto entity, const CameraComponent& cameraComponent)
	{
		if (cameraComponent.primary)
			primaryCameraEntity = { this, entity };
	});

	return primaryCameraEntity;
}

void Scene::onCameraComponentConstruction(Entity entity, CameraComponent& cameraComponent)
{
	if (m_viewportWidth != 0 && m_viewportHeight != 0)
		cameraComponent.camera.setViewportSize(m_viewportWidth, m_viewportHeight);
}

void Scene::onTagComponentCreation(Entity entity, TagComponent& tagComponent)
{
	const UUIDComponent& uuidComponent = entity.getComponent<UUIDComponent>();
	Log::cometInfo("Creating entity with UUID = {0} and tag = '{1}'", static_cast<std::string>(uuidComponent), static_cast<std::string>(tagComponent));
}

void Scene::renderModels()
{
	// Partial owning group as transform will be used in other views/groups
	const auto group = m_registry.group<ModelComponent>(entt::get<TransformComponent>);
	group.each([](const entt::entity entity, const ModelComponent& modelComponent, const TransformComponent& transformComponent)
	{
		SceneRenderer::drawModel(modelComponent.model, transformComponent.getTransform(), static_cast<int32_t>(entity));
	});
}

void Scene::renderSprites()
{
	// Partial owning group as transform will be used in other views/groups
	auto group = m_registry.group<SpriteComponent>(entt::get<TransformComponent>);

	// Sort by transform z coordinate translation so sprites are drawn in the correct order
	group.sort<TransformComponent>([](const TransformComponent& lhs, const TransformComponent& rhs)
	{
		return lhs.translation.z < rhs.translation.z;
	});

	group.each([](const entt::entity entity, const SpriteComponent& spriteComponent, const TransformComponent& transformComponent)
	{
		if (spriteComponent.spriteTextureType == SpriteComponent::TextureType::NORMAL)
			Renderer2D::drawQuad(transformComponent.getTransform(), spriteComponent.color, spriteComponent.texture, spriteComponent.tilingFactor, static_cast<int32_t>(entity));
		else
			Renderer2D::drawSubQuad(transformComponent.getTransform(), spriteComponent.color, spriteComponent.subTexture, spriteComponent.tilingFactor, static_cast<int32_t>(entity));
	});
}

SceneRenderer::PointLightList Scene::getPointLights()
{
	SceneRenderer::PointLightList pointLights;

	// Partial owning group as transform will be used in other views/groups
	const auto group = m_registry.group<PointLightComponent>(entt::get<TransformComponent>);

	group.each([&pointLights](const entt::entity entity, const PointLightComponent& pointLightComponent, const TransformComponent transformComponent)
	{
		pointLights.emplace_back(std::make_pair(pointLightComponent.pointLight, transformComponent.translation));
	});

	return pointLights;
}

}
