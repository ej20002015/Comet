#include "CometPCH.h"
#include "ScriptRegistry.h"

namespace Comet
{

void ScriptRegistry::regLibScripts()
{
	// reg needs to be called for all scripts that are part of the Comet static lib
	reg<NullScript>();
}

const std::vector<std::string>& ScriptRegistry::getScriptNames()
{
	return scriptNames();
}

MapStr2T<ScriptRegistry::CreateScriptFunc>& ScriptRegistry::scripts()
{
	static MapStr2T<CreateScriptFunc> registeredScripts;
	return registeredScripts;
}

std::vector<std::string>& ScriptRegistry::scriptNames()
{
	static std::vector<std::string> registeredScriptNames;
	return registeredScriptNames;
}

}