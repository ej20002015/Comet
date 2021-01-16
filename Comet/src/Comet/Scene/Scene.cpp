#include "CometPCH.h"
#include "Scene.h"

#include "Entity.h"
#include "Components.h"

namespace Comet
{
	Entity Scene::createEntity(const std::string& tag)
	{
		Entity newEntity(this, m_registry.create());

		//All entities will have a UUID
		newEntity.addComponent<UUIDComponent>();

		if (!tag.empty())
			newEntity.addComponent<TagComponent>(tag);

		return newEntity;
	}
}
