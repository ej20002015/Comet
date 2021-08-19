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
			node.push_back(static_cast<std::uint64_t>(rhs));
			return node;
		}

		static bool decode(const Node& node, Comet::UUID& rhs)
		{
			rhs = node.as<uint64_t>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4 rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!(node.IsSequence() && node.size() == 4))
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();

			return true;
		}
	};

	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3 rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!(node.IsSequence() && node.size() == 3))
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();

			return true;
		}
	};

	template<>
	struct convert<glm::vec2>
	{
		static Node encode(const glm::vec2 rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec2& rhs)
		{
			if (!(node.IsSequence() && node.size() == 2))
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::quat>
	{
		static Node encode(const glm::quat rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::quat& rhs)
		{
			if (!(node.IsSequence() && node.size() == 4))
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();

			return true;
		}
	};

	template<>
	struct convert<Comet::SceneCamera::ProjectionType>
	{
		static Node encode(const Comet::SceneCamera::ProjectionType rhs)
		{
			Node node;
			node.push_back(static_cast<uint32_t>(rhs));
			return node;
		}

		static bool decode(const Node& node, Comet::SceneCamera::ProjectionType& rhs)
		{
			rhs = static_cast<Comet::SceneCamera::ProjectionType>(node.as<uint32_t>());
			return true;
		}
	};

	template<>
	struct convert<Comet::TextureFilter>
	{
		static Node encode(const Comet::TextureFilter rhs)
		{
			Node node;
			node.push_back(static_cast<uint32_t>(rhs));
			return node;
		}

		static bool decode(const Node& node, Comet::TextureFilter& rhs)
		{
			rhs = static_cast<Comet::TextureFilter>(node.as<uint32_t>());
			return true;
		}
	};

	template<>
	struct convert<Comet::TextureWrap>
	{
		static Node encode(const Comet::TextureWrap rhs)
		{
			Node node;
			node.push_back(static_cast<uint32_t>(rhs));
			return node;
		}

		static bool decode(const Node& node, Comet::TextureWrap& rhs)
		{
			rhs = static_cast<Comet::TextureWrap>(node.as<uint32_t>());
			return true;
		}
	};

	template<>
	struct convert<Comet::SpriteComponent::SpriteTextureType>
	{
		static Node encode(const Comet::SpriteComponent::SpriteTextureType rhs)
		{
			Node node;
			node.push_back(static_cast<uint32_t>(rhs));
			return node;
		}

		static bool decode(const Node& node, Comet::SpriteComponent::SpriteTextureType& rhs)
		{
			rhs = static_cast<Comet::SpriteComponent::SpriteTextureType>(node.as<uint32_t>());
			return true;
		}
	};
}

namespace Comet
{

	YAML::Emitter& operator<<(YAML::Emitter& out, const UUID uuid)
	{
		out << static_cast<uint64_t>(uuid);
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

	static void deserializeUUIDComponent(const YAML::Node& componentNode, Entity entity);
	static void deserializeTagComponent(const YAML::Node& componentNode, Entity entity);
	static void deserializeTransformComponent(const YAML::Node& componentNode, Entity entity);
	static void deserializeCameraComponent(const YAML::Node& componentNode, Entity entity);
	static void deserializeSpriteComponent(const YAML::Node& componentNode, Entity entity);

	void SceneSerializer::serialize(const std::string& filepath, Reference<Scene> scene)
	{
		std::filesystem::path path(filepath);
		if (path.extension() != ".cmtscn")
		{
			Log::cometError("File '{0}' does not have the .cmtscn extension so cannot be used as a save file", filepath);
			CMT_COMET_ASSERT(false);
			return;
		}

		Log::cometInfo("Serializing scene {0}", scene->getSceneName());
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

		std::fstream fout(filepath, std::ios::out);
		if (!fout)
		{
			Log::cometError("Cannot save scene - file cannot be created/written to");
			return;
		}

		fout << out.c_str();
		fout.close();
	}

	void SceneSerializer::deserialize(const std::string& filepath, Reference<Scene> scene)
	{
		std::filesystem::path path(filepath);
		if (path.extension() != ".cmtscn")
		{
			Log::cometError("File '{0}' does not have the .cmtscn extension so cannot be opened", filepath);
			CMT_COMET_ASSERT(false);
			return;
		}

		std::ifstream fin(filepath);
		if (!fin)
		{
			Log::cometError("Cannot load scene - file cannot be opened");
			return;
		}

		std::stringstream fileString;
		fileString << fin.rdbuf();
		fin.close();

		YAML::Node inputData = YAML::Load(fileString);
		if (!inputData["Scene"])
		{
			Log::cometError("Cannot load scene - file format is incorrect");
			return;
		}

		std::string sceneName = inputData["Scene"]["Scene Name"].as<std::string>();
		scene->setSceneName(sceneName);
		Log::cometInfo("Deserializing scene \"{0}\"", sceneName);

		YAML::Node entitiesNode = inputData["Scene"]["Entities"];
		//Parse nodes in reverse so they appear in the scene heirarchy panel in the same order as the save file
		for (int32_t i = static_cast<int32_t>(entitiesNode.size() - 1); i > -1; --i)
		{
			deserializeEntityNode(entitiesNode[i], scene);
		}
	
	}

	void SceneSerializer::serializeEntity(YAML::Emitter& out, Entity entity)
	{
		out << YAML::BeginMap;
		std::string entityName = static_cast<std::string>(entity.getComponent<UUIDComponent>()) + ": " + static_cast<std::string>(entity.getComponent<TagComponent>());
		out << YAML::Key << "Entity" << YAML::Value << entityName;
		out << YAML::Key << "Components" << YAML::Value << YAML::BeginMap;

		serializeComponent<UUIDComponent>(out, entity, serializeUUIDComponent);
		serializeComponent<TagComponent>(out, entity, serializeTagComponent);
		serializeComponent<TransformComponent>(out, entity, serializeTransformComponent);
		serializeComponent<CameraComponent>(out, entity, serializeCameraComponent);
		serializeComponent<SpriteComponent>(out, entity, serializeSpriteComponent);

		out << YAML::EndMap;
		out << YAML::EndMap;
	}

	void SceneSerializer::deserializeEntityNode(const YAML::Node& entityNode, const Reference<Scene>& scene)
	{
		Entity deserializedEntity = scene->createEntity();

		deserializeComponent<UUIDComponent>(entityNode, deserializedEntity, deserializeUUIDComponent);
		deserializeComponent<TagComponent>(entityNode, deserializedEntity, deserializeTagComponent);
		deserializeComponent<TransformComponent>(entityNode, deserializedEntity, deserializeTransformComponent);
		deserializeComponent<CameraComponent>(entityNode, deserializedEntity, deserializeCameraComponent);
		deserializeComponent<SpriteComponent>(entityNode, deserializedEntity, deserializeSpriteComponent);
	}

	template<typename T, typename SerializeComponentFunction>
	void SceneSerializer::serializeComponent(YAML::Emitter& out, Entity entity, SerializeComponentFunction serializeFunction)
	{
		if (!entity.hasComponent<T>())
			return;
		
		std::string fullComponentName = std::string(typeid(T).name());
		std::string componentName = fullComponentName.substr(fullComponentName.find_last_of(":") + 1);

		out << YAML::Key << componentName << YAML::Value << YAML::BeginMap;
		serializeFunction(out, entity.getComponent<T>());
		out << YAML::EndMap;
	}

	template<typename T, typename DeserializeComponentFunction>
	void SceneSerializer::deserializeComponent(const YAML::Node& entityNode, Entity deserializedEntity, DeserializeComponentFunction deserializeFunction)
	{
		std::string fullComponentName = std::string(typeid(T).name());
		std::string componentName = fullComponentName.substr(fullComponentName.find_last_of(":") + 1);

		YAML::Node componentNode = entityNode["Components"][componentName];
		if (componentNode)
		{
			deserializeFunction(componentNode, deserializedEntity);
		}
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
		out << YAML::Key << "Camera" << YAML::Value << YAML::BeginMap;

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
		if (spriteComponent.texture)
		{
			out << YAML::Key << "Texture" << YAML::Value << YAML::BeginMap;

			//TODO: Adapt for programatically created textures if supported in the future

			out << YAML::Key << "Filepath" << YAML::Value << spriteComponent.texture->getFilepath();
			out << YAML::Key << "SRGB" << YAML::Value << spriteComponent.texture->getSRGB();
			out << YAML::Key << "Texture Mag Filter" << YAML::Value << spriteComponent.texture->getTextureMagFilter();
			out << YAML::Key << "Texture Min Filter" << YAML::Value << spriteComponent.texture->getTextureMinFilter();
			out << YAML::Key << "Texture Wrap" << YAML::Value << spriteComponent.texture->getTextureWrap();

			out << YAML::EndMap;
		}


		out << YAML::Key << "Color" << YAML::Value << spriteComponent.color;
		out << YAML::Key << "Tiling Factor" << YAML::Value << spriteComponent.tilingFactor;
		out << YAML::Key << "Sprite Texture Type" << YAML::Value << spriteComponent.spriteTextureType;

		if (spriteComponent.texture)
		{
			out << YAML::Key << "Sub Texture" << YAML::Value << YAML::BeginMap;

			out << YAML::Key << "Texture Atlas Cell Size" << YAML::Value << spriteComponent.subTexture.getCellSize();
			out << YAML::Key << "Texture Atlas Index" << YAML::Value << spriteComponent.subTexture.getTextureAtlasIndex();
			out << YAML::Key << "Texture Scale" << YAML::Value << spriteComponent.subTexture.getTextureScale();

			out << YAML::EndMap;
		}
	}

	void deserializeUUIDComponent(const YAML::Node& componentNode, Entity entity)
	{
		UUID uuid = componentNode["UUID"].as<UUID>();
		entity.getComponent<UUIDComponent>().ID = uuid;
	}

	void deserializeTagComponent(const YAML::Node& componentNode, Entity entity)
	{
		std::string tag = componentNode["Tag"].as<std::string>();
		entity.getComponent<TagComponent>().tag = tag;
	}

	void deserializeTransformComponent(const YAML::Node& componentNode, Entity entity)
	{
		glm::vec3 translation = componentNode["Translation"].as<glm::vec3>();
		glm::quat rotation = componentNode["Rotation"].as<glm::quat>();
		glm::vec3 scale = componentNode["Scale"].as<glm::vec3>();

		TransformComponent& transformComponent = entity.getComponent<TransformComponent>();
		transformComponent.translation = translation;
		transformComponent.rotation = rotation;
		transformComponent.scale = scale;
	}

	void deserializeCameraComponent(const YAML::Node& componentNode, Entity entity)
	{
		YAML::Node sceneCameraNode = componentNode["Camera"];

		SceneCamera::ProjectionType projectionType = sceneCameraNode["Projection Type"].as<SceneCamera::ProjectionType>();
		float aspectRatio = sceneCameraNode["Aspect Ratio"].as<float>();
		bool fixedAspectRatio = sceneCameraNode["Fixed Aspect Ratio"].as<bool>();
		float perspectiveFOV = sceneCameraNode["Perspective FOV"].as<float>();
		float perspectiveNearPlane = sceneCameraNode["Perspective Near Plane"].as<float>();
		float perspectiveFarPlane = sceneCameraNode["Perspective Far Plane"].as<float>();
		float orthographicSize = sceneCameraNode["Orthographic Size"].as<float>();
		float orthographicNearPlane = sceneCameraNode["Orthographic Near Plane"].as<float>();
		float orthographicFarPlane = sceneCameraNode["Orthographic Far Plane"].as<float>();

		bool primary = componentNode["Primary"].as<bool>();

		CameraComponent& cameraComponent = entity.addComponent<CameraComponent>();

		SceneCamera& sceneCamera = cameraComponent.camera;
		sceneCamera.setProjectionType(projectionType);
		if (fixedAspectRatio)
			sceneCamera.setAspectRatio(aspectRatio);
		sceneCamera.setFixedAspectRatio(fixedAspectRatio);
		sceneCamera.setPerspectiveFOV(perspectiveFOV);
		sceneCamera.setPerspectiveNearPlane(perspectiveNearPlane);
		sceneCamera.setPerspectiveFarPlane(perspectiveFarPlane);
		sceneCamera.setOrthographicSize(orthographicSize);
		sceneCamera.setOrthographicNearPlane(orthographicNearPlane);
		sceneCamera.setOrthographicFarPlane(orthographicFarPlane);

		cameraComponent.primary = primary;
	}

	void deserializeSpriteComponent(const YAML::Node& componentNode, Entity entity)
	{
		YAML::Node textureNode = componentNode["Texture"];

		Reference<Texture2D> texture = nullptr;
		if (textureNode)
		{
			std::string filepath = textureNode["Filepath"].as<std::string>();
			bool SRGB = textureNode["SRGB"].as<bool>();
			TextureFilter textureMagFilter = textureNode["Texture Mag Filter"].as<TextureFilter>();
			TextureFilter textureMinFilter = textureNode["Texture Min Filter"].as<TextureFilter>();
			TextureWrap textureWrap = textureNode["Texture Wrap"].as<TextureWrap>();

			texture = Texture2D::create(filepath, SRGB, textureMagFilter, textureMinFilter, textureWrap);
		}

		glm::vec4 color = componentNode["Color"].as<glm::vec4>();
		float tilingFactor = componentNode["Tiling Factor"].as<float>();
		SpriteComponent::SpriteTextureType spriteTextureType = componentNode["Sprite Texture Type"].as<SpriteComponent::SpriteTextureType>();

		YAML::Node subTextureNode = componentNode["Sub Texture"];

		if (subTextureNode)
		{
			uint32_t textureAtlasCellSize = subTextureNode["Texture Atlas Cell Size"].as<uint32_t>();
			glm::vec2 textureAtlasIndex = subTextureNode["Texture Atlas Index"].as<glm::vec2>();
			glm::vec2 textureScale = subTextureNode["Texture Scale"].as<glm::vec2>();

			entity.addComponent<SpriteComponent>(texture, color, tilingFactor, spriteTextureType, textureAtlasCellSize, textureAtlasIndex, textureScale);
			return;
		}

		SpriteComponent& spriteComponent = entity.addComponent<SpriteComponent>();
		spriteComponent.color = color;
		spriteComponent.tilingFactor = tilingFactor;
		spriteComponent.spriteTextureType = spriteTextureType;
	}

}