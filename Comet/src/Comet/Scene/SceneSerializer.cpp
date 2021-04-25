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

	static void serializeUUIDComponent(YAML::Emitter& out, const UUIDComponent& UUID);
	static void serializeTagComponent(YAML::Emitter& out, const TagComponent& tagComponent);
	static void serializeTransformComponent(YAML::Emitter& out, const UUIDComponent& UUID);
	static void serializeCameraComponent(YAML::Emitter& out, const UUIDComponent& UUID);
	static void serializeSpriteComponent(YAML::Emitter& out, const UUIDComponent& UUID);

	void SceneSerializer::serialize(const std::string& filepath, Reference<Scene> scene)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << YAML::BeginMap;;
		out << YAML::Key << "Scene Name" << YAML::Value << "Unnamed";
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
		/*serializeComponent<TransformComponent>();
		serializeComponent<CameraComponent>();
		serializeComponent<SpriteComponent>();*/

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
		out << YAML::Key << "Tag" << YAML::Value << static_cast<std::string>(tagComponent.tag);
	}

}
