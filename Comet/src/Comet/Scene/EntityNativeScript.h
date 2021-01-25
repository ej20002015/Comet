#pragma once
#include "CometPCH.h"

#include "Entity.h"

namespace Comet
{

	class EntityNativeScript
	{
	public:
		EntityNativeScript() = default;
		virtual ~EntityNativeScript() = default;

	protected:
		virtual void onStart() {}
		virtual void onUpdate(Timestep ts) {}
		virtual void onEnd() {}

		template<typename T>
		bool hasComponent()
		{
			return m_entity.hasComponent<T>();
		}

		template<typename T>
		T& getComponent()
		{
			return m_entity.getComponent<T>();
		}

	private:
		Entity m_entity;

		friend class Scene;
	};

}