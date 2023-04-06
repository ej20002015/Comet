#pragma once
#include "CometPCH.h"

namespace Comet
{

class ComponentUtils
{
public:
	template<typename T>
	static std::string getComponentName()
	{
		const std::string fullComponentName = std::string(typeid(T).name());
		return fullComponentName.substr(fullComponentName.find_last_of(":") + 1);
	}
};

}