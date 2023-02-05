#pragma once

#include "yaml-cpp/yaml.h"

#include "Scene.h"
#include "Entity.h"

namespace Comet
{

class SceneSerializer
{
public:
	static constexpr const char* const FILE_EXTENSION = ".cmtscn";

	static void serialize(const std::filesystem::path& filepath, Reference<Scene> scene);
	static void deserialize(const std::filesystem::path& filepath, Reference<Scene> scene);

private:
	static void serializeEntity(YAML::Emitter& out, Entity entity);
	static void deserializeEntityNode(const YAML::Node& entityNode, const Reference<Scene>& scene);

	template<typename T, typename SerializeComponentFunction>
	static void serializeComponent(YAML::Emitter& out, Entity entity, SerializeComponentFunction serializeFunction);

	template<typename T, typename DeserializeComponentFunction>
	static void deserializeComponent(const YAML::Node& entityNode, Entity deserializedEntity, DeserializeComponentFunction deserializeFunction);
};

}