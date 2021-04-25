#pragma once

#include "yaml-cpp/yaml.h"

#include "Scene.h"
#include "Entity.h"

namespace Comet
{

	class SceneSerializer
	{
	public:
		static void serialize(const std::string& filepath, Reference<Scene> scene);
		static void deserialize(const std::string& filepath, Reference<Scene> scene);

	private:
		static void serializeEntity(YAML::Emitter& out, Entity entity);

		template<typename T, typename SerializeComponentFunction>
		static void serializeComponent(YAML::Emitter& out, Entity entity, SerializeComponentFunction serializeFunction);
	};

}