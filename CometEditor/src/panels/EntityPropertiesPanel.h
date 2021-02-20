#pragma once

#include <Comet.h>

#include "imgui.h"

namespace Comet
{

	class EntityPropertiesPanel
	{
	public:
		EntityPropertiesPanel() = default;

		void setEntity(Entity entity) { m_entity = entity; }

		void onImGuiRender();

	private:
		template<typename T, typename ComponentUIFunction>
		void componentImGuiRender(const std::string& headerName, ComponentUIFunction componentUIFunction);

	private:
		Entity m_entity;
	};

}
