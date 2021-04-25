#pragma once

#include "Scene.h"

namespace Comet
{

	class SceneSerializer
	{
	public:
		static void Serialize(const std::string& filepath, const Reference<Scene>& scene);
		static void Deserialize(const std::string& filepath, const Reference<Scene>& scene);	
	};

}