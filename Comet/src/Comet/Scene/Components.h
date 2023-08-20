#pragma once
#include "CometPCH.h"

#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtc/type_ptr.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/matrix_decompose.hpp"

#include "Comet/Core/UUID.h"
#include "SceneCamera.h"
#include "EntityNativeScript.h"

#include "Comet/Renderer/Material.h"
#include "Comet/Renderer/Texture.h"
#include "Comet/Renderer/Model.h"
#include "Comet/Renderer/ModelFactory.h"
#include "Comet/Renderer/PointLight.h"

namespace Comet
{

struct UUIDComponent
{
	UUID ID;

	UUIDComponent() = default;

	UUIDComponent(const UUIDComponent& other) = default;
	~UUIDComponent() = default;

	operator const UUID() const { return ID; }
	operator const uint64_t() const { return ID; }
	operator const std::string() const { return ID; }
};

struct TagComponent
{
	std::string tag = "Unnamed Entity";

	TagComponent() = default;

	TagComponent(const std::string_view tag)
		: tag(tag) {}

	TagComponent(const TagComponent& other) = default;
	~TagComponent() = default;

	operator std::string&() { return tag; }
	operator const std::string&() const { return tag; }
};

//TODO: FIX NON INDEPENDENT ADJUSTMENT OF ROTATION COMPONENTS
struct TransformComponent
{
	glm::vec3 translation = glm::vec3(0.0f);
	glm::quat rotation = glm::quat();
	glm::vec3 scale = glm::vec3(1.0f);

	TransformComponent() = default;
	TransformComponent(const glm::vec3& translation)
		: translation(translation) {}

	TransformComponent(const TransformComponent& other) = default;
	~TransformComponent() = default;

	glm::mat4 getTransform() const
	{
		return glm::translate(glm::mat4(1.0f), translation) * glm::toMat4(rotation) * glm::scale(glm::mat4(1.0f), { scale.x, scale.y, scale.z });
	}

	void setTransform(const glm::mat4& transformMatrix)
	{
		glm::vec3 skew;
		glm::vec4 perspective;
		glm::decompose(transformMatrix, scale, rotation, translation, skew, perspective);
	}

	operator glm::mat4() const { return getTransform(); }
};

struct CameraComponent
{
	SceneCamera camera;
	bool primary = false;

	CameraComponent() = default;
	CameraComponent(const bool primary)
		: primary(primary) {}

	CameraComponent(const CameraComponent& other) = default;
	~CameraComponent() = default;
};

//Wrapper for a script - lifetime is controlled at runtime
struct NativeScriptComponent
{
	std::string scriptName = NullScript::getName();
	EntityNativeScript* script = nullptr;

	NativeScriptComponent() = default;

	NativeScriptComponent(const NativeScriptComponent& other) = default;
	~NativeScriptComponent() = default;
};

struct SpriteComponent
{
	Reference<Texture2D> texture = nullptr;
	glm::vec4 color = glm::vec4(1.0f);
	float tilingFactor = 1.0f;

	enum class TextureType
	{
		NORMAL = 0,
		SUB_TEXTURE
	};

	TextureType spriteTextureType = TextureType::NORMAL;

	//Data used if sprite texture is a sub-texture of a texture atlas
	Texture2DSubTexture subTexture;

	SpriteComponent() = default;

	SpriteComponent(const Reference<Texture2D>& texture, const glm::vec4& color = glm::vec4(1.0f), const float tilingFactor = 1.0f,
		const TextureType spriteTextureType = TextureType::NORMAL, const uint32_t textureAtlasCellSize = 0,
		const glm::vec2& textureAtlasIndex = glm::vec2(0.0f), const glm::vec2& subTextureScale = glm::vec2(0.0f))
		: texture(texture), color(color), tilingFactor(tilingFactor), spriteTextureType(spriteTextureType),
			subTexture(texture, textureAtlasCellSize, textureAtlasIndex, subTextureScale) {}

	SpriteComponent(const SpriteComponent& other) = default;
	~SpriteComponent() = default;
};

struct ModelComponent
{
	Reference<Model> model = ModelFactory::create(ModelFactory::MeshType::CUBE);

	ModelComponent() = default;
	ModelComponent(const Reference<Model>& model)
		: model(model) {}
};

struct PointLightComponent
{
	Reference<PointLight> pointLight = PointLight::create();

	PointLightComponent() = default;
	PointLightComponent(const Reference<PointLight>& pointLight)
		: pointLight(pointLight) {}
};

}