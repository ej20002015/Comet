#pragma once

#include <Comet.h>

namespace Comet
{

	class EntityPropertiesPanel
	{
	public:

		enum EntityOptionsFlags
		{
			NONE = 0,
			CAN_DELETE_ENTITY = CMT_BIT(0)
		};

		EntityPropertiesPanel();

		void setEntity(Entity entity) { m_entity = entity; }

		void onImGuiRender();

	private:
		template<typename T, typename ComponentUIFunction>
		void componentImGuiRender(const std::string& headerName, ComponentUIFunction componentUIFunction, EntityOptionsFlags optionsFlags = EntityOptionsFlags::CAN_DELETE_ENTITY);

	private:
		Entity m_entity;
		Reference<Texture2D> m_optionsTexture;
	};

}
