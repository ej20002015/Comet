#pragma once
#include "CometPCH.h"

#include "glm/glm.hpp"

#include "Comet/Core/UUID.h"
#include "SceneCamera.h"
#include "EntityNativeScript.h"

#include "Comet/Renderer/Texture.h"

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

		TagComponent(const std::string& tag)
			: tag(tag) {}

		TagComponent(const TagComponent& other) = default;
		~TagComponent() = default;

		operator std::string&() { return tag; }
		operator const std::string&() const { return tag; }
	};

	struct TransformComponent
	{
		glm::mat4 transform = glm::mat4(1.0f);

		TransformComponent() = default;
		TransformComponent(const glm::mat4& transform)
			: transform(transform) {}

		TransformComponent(const TransformComponent& other) = default;
		~TransformComponent() = default;

		operator glm::mat4&() { return transform; }
		operator const glm::mat4&() const { return transform; }
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

	struct SpriteSubComponent
	{
		Reference<Texture2DSubTexture> subTexture = nullptr;
		glm::vec4 colorTint = glm::vec4(1.0f);
		float tilingFactor = 1.0f;

		SpriteSubComponent() = default;

		SpriteSubComponent(Reference<Texture2DSubTexture> subTexture, const glm::vec4& colorTint = glm::vec4(1.0f), float tilingFactor = 1.0f)
			: subTexture(subTexture), colorTint(colorTint), tilingFactor(tilingFactor) {}

		SpriteSubComponent(const SpriteSubComponent& other) = default;
		~SpriteSubComponent() = default;
	};

}