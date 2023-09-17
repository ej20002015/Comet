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
	bool hasComponent() const
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

#define DEF_SCRIPT(TYPE) static bool s_registered; \
	static constexpr const char* const s_name = #TYPE; \
	static const std::string getName() { return s_name; } \
	friend class ScriptRegistry; \

class NullScript : public EntityNativeScript
{
public:
	DEF_SCRIPT(NullScript)
};

}