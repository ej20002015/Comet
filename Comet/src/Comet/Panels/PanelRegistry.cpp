#include "CometPCH.h"
#include "PanelRegistry.h"

#include "EntityPropertiesPanel.h"
#include "SceneHierarchyPanel.h"

namespace Comet
{

void PanelRegistry::regLibPanels()
{
	// reg needs to be called for all panels that are part of the Comet static lib
	reg<EntityPropertiesPanel>();
	reg<SceneHierarchyPanel>();
}

std::unordered_map<std::type_index, PanelRegistry::CreatePanelFunc>& PanelRegistry::panels()
{
	static std::unordered_map<std::type_index, CreatePanelFunc> s_registeredPanels;
	return s_registeredPanels;
}


}