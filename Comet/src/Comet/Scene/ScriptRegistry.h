#pragma once
#include "CometPCH.h"

#include "EntityNativeScript.h"
#include "Comet/Core/Utilities.h"

#include <type_traits>

namespace Comet
{

class ScriptRegistry
{
public:
	using CreateScriptFunc = std::function<EntityNativeScript*()>;

public:
	ScriptRegistry() = delete;

	// Needs to be called because the scripts in this static lib have to be verbosely registered
	static void regLibScripts();

	template<std::derived_from<EntityNativeScript> T>
	static bool reg()
	{
		if (const auto iter = scripts().find(T::s_name); iter == scripts().end())
		{
			scripts()[T::s_name] = []() { return new T(); };
			scriptNames().push_back(T::s_name);
		}

		return true;
	}

	static EntityNativeScript* createRegisteredScript(const std::string& scriptName)
	{
		CMT_COMET_ASSERT_MESSAGE(scripts().find(scriptName) != scripts().end(), "Given script is not registered");
		return scripts()[scriptName]();
	}

	static const std::vector<std::string>& getScriptNames();

private:
	static MapStr2T<CreateScriptFunc>& scripts();
	static std::vector<std::string>& scriptNames();
};

#define REG_SCRIPT(TYPE) \
	bool TYPE::s_registered = ScriptRegistry::reg<TYPE>(); \

}