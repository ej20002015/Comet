#include "CometPCH.h"
#include "SceneSerializer.h"

#include "Components.h"

namespace YAML
{

	template<>
	struct convert<Comet::UUID>
	{
		static Node encode(const Comet::UUID rhs)
		{
			Node node;
			node.push_back(static_cast<std::string>(rhs));
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			//TODO: Do this
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

}

namespace Comet
{

	YAML::Emitter& operator<<(YAML::Emitter& out, const UUID uuid)
	{
		out << static_cast<std::string>(uuid);
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& vec4)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << vec4.x << vec4.y << vec4.z << vec4.w << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& vec3)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << vec3.x << vec3.y << vec3.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& vec2)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << vec2.x << vec2.y << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::quat& quat)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << quat.x << quat.y << quat.z << quat.w << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const SceneCamera::ProjectionType& projectionType)
	{
		out << static_cast<uint32_t>(projectionType);
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const TextureFilter& textureFilter)
	{
		out << static_cast<uint32_t>(textureFilter);
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const TextureWrap& textureWrap)
	{
		out << static_cast<uint32_t>(textureWrap);
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const SpriteComponent::SpriteTextureType& spriteTextureType)
	{
		out << static_cast<uint32_t>(spriteTextureType);
		return out;
	}

	static void serializeUUIDComponent(YAML::Emitter& out, const UUIDComponent& UUID);
	static void serializeTagComponent(YAML::Emitter& out, const TagComponent& tagComponent);
	static void serializeTransformComponent(YAML::Emitter& out, const TransformComponent& transformComponent);
	static void serializeCameraComponent(YAML::Emitter& out, const CameraComponent& cameraComponent);
	static void serializeSpriteComponent(YAML::Emitter& out, const SpriteComponent& spriteComponent);

	void SceneSerializer::serialize(const std::string& filepath, Reference<Scene> scene)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << YAML::BeginMap;;
		out << YAML::Key << "Scene Name" << YAML::Value << scene->getSceneName();
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

		//Serialize the entities
		scene->m_registry.each([&out, scene](entt::entity entityHandle)
		{
			Entity entity(scene.get(), entityHandle);
			if (!entity)
				return;

			serializeEntity(out, entity);
		});

		out << YAML::EndSeq;
		out << YAML::EndMap;
		out << YAML::EndMap;

		std::fstream fout(filepath);
		fout << out.c_str();
	}

	void SceneSerializer::deserialize(const std::string& filepath, Reference<Scene> scene)
	{
	}

	void SceneSerializer::serializeEntity(YAML::Emitter& out, Entity entity)
	{
		out << YAML::BeginMap;
		//TODO: Provide an encoding and decoding for UUID type
		out << YAML::Key << "Entity" << YAML::Value << static_cast<std::string>(entity.getComponent<UUIDComponent>()) + ": " + static_cast<std::string>(entity.getComponent<TagComponent>());
		out << YAML::Key << "Components" << YAML::Value << YAML::BeginSeq;

		serializeComponent<UUIDComponent>(out, entity, serializeUUIDComponent);
		serializeComponent<TagComponent>(out, entity, serializeTagComponent);
		serializeComponent<TransformComponent>(out, entity, serializeTransformComponent);
		serializeComponent<CameraComponent>(out, entity, serializeCameraComponent);
		serializeComponent<SpriteComponent>(out, entity, serializeSpriteComponent);

		out << YAML::EndSeq;
		out << YAML::EndMap;
	}

	template<typename T, typename SerializeComponentFunction>
	void SceneSerializer::serializeComponent(YAML::Emitter& out, Entity entity, SerializeComponentFunction serializeFunction)
	{
		if (!entity.hasComponent<T>())
			return;
		
		std::string fullComponentName = std::string(typeid(T).name());
		std::string componentName = fullComponentName.substr(fullComponentName.find_last_of(":") + 1);

		out << YAML::BeginMap;
		out << YAML::Key << componentName << YAML::Value << YAML::BeginSeq;
		out << YAML::BeginMap;
		serializeFunction(out, entity.getComponent<T>());
		out << YAML::EndMap;
		out << YAML::EndSeq;
		out << YAML::EndMap;
	}

	void serializeUUIDComponent(YAML::Emitter& out, const UUIDComponent& UUID)
	{
		out << YAML::Key << "UUID" << YAML::Value << UUID.ID;
	}

	void serializeTagComponent(YAML::Emitter& out, const TagComponent& tagComponent)
	{
		out << YAML::Key << "Tag" << YAML::Value << tagComponent.tag;
	}

	static void serializeTransformComponent(YAML::Emitter& out, const TransformComponent& transformComponent)
	{
		out << YAML::Key << "Translation" << YAML::Value << transformComponent.translation;
		out << YAML::Key << "Rotation" << YAML::Value << transformComponent.rotation;
		out << YAML::Key << "Scale" << YAML::Value << transformComponent.scale;
	}

	static void serializeCameraComponent(YAML::Emitter& out, const CameraComponent& cameraComponent)
	{
		out << YAML::Key << "Scene Camera" << YAML::Value << YAML::BeginMap;

		out << YAML::Key << "Projection Type" << YAML::Value << cameraComponent.camera.getProjectionType();
		out << YAML::Key << "Aspect Ratio" << YAML::Value << cameraComponent.camera.getAspectRatio();
		out << YAML::Key << "Fixed Aspect Ratio" << YAML::Value << cameraComponent.camera.getFixedAspectRatio();
		out << YAML::Key << "Perspective FOV" << YAML::Value << cameraComponent.camera.getPerspectiveFOV();
		out << YAML::Key << "Perspective Near Plane" << YAML::Value << cameraComponent.camera.getPerspectiveNearPlane();
		out << YAML::Key << "Perspective Far Plane" << YAML::Value << cameraComponent.camera.getPerspectiveFarPlane();
		out << YAML::Key << "Orthographic Size" << YAML::Value << cameraComponent.camera.getOrthographicSize();
		out << YAML::Key << "Orthographic Near Plane" << YAML::Value << cameraComponent.camera.getOrthographicNearPlane();
		out << YAML::Key << "Orthographic Far Plane" << YAML::Value << cameraComponent.camera.getOrthographicFarPlane();

		out << YAML::EndMap;

		out << YAML::Key << "Primary" << YAML::Value << cameraComponent.primary;
	}

	static void serializeSpriteComponent(YAML::Emitter& out, const SpriteComponent& spriteComponent)
	{
		out << YAML::Key << "Texture" << YAML::Value << YAML::BeginMap;

		//TODO: Adapt for programatically created textures if supported in the future

		out << YAML::Key << "Filepath" << YAML::Value << spriteComponent.texture->getFilepath();
		out << YAML::Key << "SRGB" << YAML::Value << spriteComponent.texture->getSRGB();
		out << YAML::Key << "Texture Mag Filter" << YAML::Value << spriteComponent.texture->getTextureMagFilter();
		out << YAML::Key << "Texture Min Filter" << YAML::Value << spriteComponent.texture->getTextureMinFilter();
		out << YAML::Key << "Texture Wrap" << YAML::Value << spriteComponent.texture->getTextureWrap();

		out << YAML::EndMap;

		out << YAML::Key << "Color" << YAML::Value << spriteComponent.color;
		out << YAML::Key << "Tiling Factor" << YAML::Value << spriteComponent.tilingFactor;
		out << YAML::Key << "Sprite Texture Type" << YAML::Value << spriteComponent.spriteTextureType;

		out << YAML::Key << "Sub Texture" << YAML::Value << YAML::BeginMap;

		out << YAML::Key << "Texture Atlas Cell Size" << YAML::Value << spriteComponent.subTexture.getCellSize();
		out << YAML::Key << "m_textureAtlasIndex" << YAML::Value << spriteComponent.subTexture.getTextureAtlasIndex();
		out << YAML::Key << "m_textureScale" << YAML::Value << spriteComponent.subTexture.getTextureScale();

		out << YAML::EndMap;
	}

}
