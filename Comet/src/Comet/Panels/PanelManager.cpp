#include "CometPCH.h"
#include "PanelManager.h"

namespace Comet
{

void PanelManager::render()
{
	for (auto& [typeID, managedPanel] : m_panels)
	{
		if (managedPanel.isVisible)
			managedPanel.panel->onImGuiRender();
	}
}

}