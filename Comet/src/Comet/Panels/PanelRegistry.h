#pragma once
#include "CometPCH.h"

#include "Panel.h"

#include <type_traits>

namespace Comet
{

class PanelRegistry
{
public:
	PanelRegistry() = delete;

	// Needs to be called because the panels in this static lib have to be verbosely registered
	static void regLibPanels();

	template<std::derived_from<Panel> T>
	static bool reg()
	{
		if (const auto iter = panels().find(typeid(T)); iter == panels().end())
			panels()[typeid(T)] = []() { return createReference<T>(); };

		return true;
	}

	template<std::derived_from<Panel> T>
	static Reference<Panel> createRegisteredPanel()
	{
		CMT_COMET_ASSERT_MESSAGE(panels().find(typeid(T)) != panels().end(), "Given panel is not registered");
		return panels()[typeid(T)]();
	}

private:
	using CreatePanelFunc = std::function<Reference<Panel>()>;

	static std::unordered_map<std::type_index, CreatePanelFunc>& panels();
};

#define REG_PANEL(TYPE) \
	bool TYPE::s_registered = PanelRegistry::reg<TYPE>(); \

}