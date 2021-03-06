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

#include "Comet/Renderer/Texture.h"

namespace Comet
{
	//TODO: Create an entity with a given UUID
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

		TagComponent(const std::string& tag)
			: tag(tag) {}

		TagComponent(const TagComponent& other) = default;
		~TagComponent() = default;

		operator std::string&() { return tag; }
		operator const std::string&() const { return tag; }
	};

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
		CameraComponent(bool primary)
			: primary(primary) {}

		CameraComponent(const CameraComponent& other) = default;
		~CameraComponent() = default;
	};

	//Wrapper for a script - lifetime is controlled at runtime
	struct NativeScriptComponent
	{
		EntityNativeScript* script = nullptr;

		//Construction and Descruction occurs on runtime start and end
		EntityNativeScript* (*constructScript)() = nullptr;
		void (*destroyScript)(NativeScriptComponent&) = nullptr;

		NativeScriptComponent() = default;

		NativeScriptComponent(const NativeScriptComponent& other) = default;
		~NativeScriptComponent() = default;

		template<typename T>
		void bind()
		{
			constructScript = []() -> EntityNativeScript*
			{
				T* script = new T();
				return static_cast<EntityNativeScript*>(script);
			};

			destroyScript = [](NativeScriptComponent& nativeScriptComponent)
			{
				delete nativeScriptComponent.script;
				nativeScriptComponent.script = nullptr;
			};
		}
	};

	struct SpriteComponent
	{
		Reference<Texture2D> texture = nullptr;
		glm::vec4 color = glm::vec4(1.0f);
		float tilingFactor = 1.0f;

		enum class SpriteTextureType
		{
			NORMAL, SUB_TEXTURE
		};

		SpriteTextureType spriteTextureType = SpriteTextureType::NORMAL;

		//Data used if sprite texture is a sub-texture of a texture atlas
		Texture2DSubTexture subTexture;

		SpriteComponent() = default;

		SpriteComponent(const Reference<Texture2D>& texture, const glm::vec4& color = glm::vec4(1.0f), float tilingFactor = 1.0f, SpriteTextureType spriteTextureType = SpriteTextureType::NORMAL, uint32_t textureAtlasCellSize = 0, const glm::vec2& textureAtlasIndex = glm::vec2(0.0f), const glm::vec2& subTextureScale = glm::vec2(0.0f))
			: texture(texture), color(color), tilingFactor(tilingFactor), spriteTextureType(spriteTextureType), subTexture(texture, textureAtlasCellSize, textureAtlasIndex, subTextureScale) {}

		SpriteComponent(const SpriteComponent& other) = default;
		~SpriteComponent() = default;
	};

}