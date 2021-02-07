#include "EntityPropertiesPanel.h"

namespace Comet
{

	void EntityPropertiesPanel::onImGuiRender()
	{
		ImGui::Begin("Entity Properties");

		if (!m_entity)
		{
			ImGui::Text("No Entity Selected");
			ImGui::End();
			return;
		}

		const std::string& tag = m_entity.getComponent<TagComponent>();
		ImGui::Text("%s", tag.c_str());

		ImGui::End();
	}

}
