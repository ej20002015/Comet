#pragma once
#include "CometPCH.h"

#include "Comet/Scene/Entity.h"
#include "Comet/Scene/Components.h"
#include "Comet/Renderer/Texture.h"

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
		static const float s_labelColumnWidth;

		Reference<Texture2D> m_noTextureIcon;
	};

}
