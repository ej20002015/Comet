#pragma once
#include "CometPCH.h"

#include "entt.hpp"

namespace Comet
{
	class Entity;

	class Scene
	{
	public:
		Scene() = default;
		static Reference<Scene> create() { return createReference<Scene>(); }

		Entity createEntity(const std::string& tag = "");

	private:
		entt::registry m_registry;

		friend class Entity;
	};

}