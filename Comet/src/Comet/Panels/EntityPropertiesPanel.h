#pragma once
#include "CometPCH.h"

#include "Panel.h"
#include "Comet/Scene/Entity.h"
#include "Comet/Scene/Components.h"
#include "Comet/Renderer/Texture.h"

#include "imgui.h"

namespace Comet
{

class EntityPropertiesPanel : public Panel
{
public:
	DEF_PANEL()

	enum ComponentOptionsFlags
	{
		NONE = 0,
		CAN_DELETE_COMPONENT = CMT_BIT(0)
	};

	EntityPropertiesPanel();


	void setEntity(Entity entity) { m_entity = entity; }

	void onImGuiRender() override;

private:
	static void initImageFileFilter();

	template<typename T, typename ComponentUIFunction>
	void componentImGuiRender(const std::string_view headerName, const ComponentUIFunction& componentUIFunction, const ComponentOptionsFlags optionsFlags = ComponentOptionsFlags::CAN_DELETE_COMPONENT);
	template<typename T, typename ComponentUIFunction>
	void renderStandardComponent(const std::string_view headerName, const ComponentUIFunction& componentUIFunction);
	template<typename T, typename ComponentUIFunction>
	void renderComponentWithOptions(const std::string_view headerName, const ComponentUIFunction& componentUIFunction, const ComponentOptionsFlags optionsFlags);

private:
	static constexpr ImGuiTreeNodeFlags NODE_FLAGS = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

	static Buffer s_imgFileFilter;

	Entity m_entity;
	static const float s_labelColumnWidth;

	Reference<Texture2D> m_noTextureIcon;
};

}
