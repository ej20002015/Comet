#pragma once
#include "CometPCH.h"

#include "Comet/Scene/Entity.h"
#include "Comet/Scene/Components.h"

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

		EntityPropertiesPanel() = default;

		void setEntity(Entity entity) { m_entity = entity; }

		void onImGuiRender();

	private:
		template<typename T, typename ComponentUIFunction>
		void componentImGuiRender(const std::string& headerName, ComponentUIFunction componentUIFunction, EntityOptionsFlags optionsFlags = EntityOptionsFlags::CAN_DELETE_ENTITY);

		//UI method for each Component
		void ImGuiRenderUUIDComponentWidget(const UUIDComponent& UUID);
		void ImGuiRenderTagComponentWidget(TagComponent& tagComponent);
		void ImGuiRenderTransformComponentWidget(TransformComponent& transformComponent);
		void ImGuiRenderCameraComponentWidget(CameraComponent& cameraComponent);
		void ImGuiRenderSpriteComponentWidget(SpriteComponent& spriteComponent);

	private:
		Entity m_entity;
		static const float s_labelColumnWidth;
	};

}
