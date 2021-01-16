#pragma once
#include "CometPCH.h"

#include "glm/glm.hpp"

#include "Comet/Core/UUID.h"

#include <iomanip>

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
		std::string tag;

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

}