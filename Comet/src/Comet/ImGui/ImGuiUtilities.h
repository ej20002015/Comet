#pragma once
#include "CometPCH.h"

#include "ImGui.h"
#include "glm/glm.hpp"

#include <filesystem>
#include <ranges>

namespace Comet
{

class ImGuiLayer;

class ImGuiUtilities
{
public:

	//Start from 1 as index 0 is being used by ImGui
	enum ImGuiFontType
	{
		FONT_NORMAL = 1,
		FONT_BOLD,
		FONT_NORMAL_ITALIC,
		FONT_BOLD_ITALIC
	};

public:
	ImGuiUtilities() = delete;

	//Dockspace
	static void beginDockspace();
	static void endDockspace();

	//Window Size
	static glm::vec2 getMinimumWindowSize();
	static void setMinimumWindowSize(const glm::vec2& size);

	//Window Padding
	static glm::vec2 getWindowPadding();
	static void setWindowPadding(const glm::vec2& padding);

	//Font Management
	static void loadFont(const std::filesystem::path& filepath, const float fontSize, const ImGuiFontType fontType);
	static void setDefaultFont(const ImGuiFontType fontType);

	static ImFont* getFont(const ImGuiFontType fontType);

	static void pushFont(const ImGuiFontType fontType);
	static void popFont();

	//Window Colours
	static void setDarkModeColors();

	//Style Vars
	static void pushAlphaFactorStyleVariable(const float alphaFactor);
	static void popStyleVariable();

	//Helper Methods
	static void seperator();
	static void pushButtonDisabled();
	static void popButtonDisabled();

	//Context IDs
	static void pushID();
	static void popID();

	//Property Grid
	static void beginPropertyGrid();
	static void endPropertyGrid();

	//Property Elements
	static void property(const std::string_view label, const std::string_view value);
	static bool property(const std::string_view label, std::string& value);

	static bool property(const std::string_view label, int32_t& value, const float delta = 1.0f, const std::string_view format = "%d", const int32_t min = 0, const int32_t max = 0);
	static bool propertySlider(const std::string_view label, int32_t& value, const std::string_view format = "%d", const int32_t min = 0, const int32_t max = 0);

	static bool property(const std::string_view label, float& value, const float delta = 0.1f, const std::string_view format = "%.1f", const float min = 0.0f, const float max = 0.0f);
	static bool property(const std::string_view label, glm::vec2& value, const float delta = 0.1f, const std::string_view format = "%.1f", const float min = 0.0f, const float max = 0.0f);
	static bool property(const std::string_view label, glm::vec3& value, const float delta = 0.1f, const std::string_view format = "%.1f", const float min = 0.0f, const float max = 0.0f);

	static bool propertyColorPicker(const std::string_view label, glm::vec4& value);
	static bool propertyColorPicker(const std::string_view label, glm::vec3 & value);

	static bool propertyButton(const std::string_view label, const std::string_view buttonText);
	static bool propertyImageButton(const std::string_view label, const uint32_t textureRendererID, const glm::vec2& size);

	static bool property(const std::string_view label, bool& value);

	template<typename StrType>
	static bool property(const std::string_view label, const std::span<const StrType> options, StrType& selected);

	template<typename TEnum>
	static bool property(const std::string_view label, const std::span<const std::string_view> options, TEnum& selected);

private:
	//Item IDs
	static char* generateItemID();
	static void propertyLabel(const std::string_view label);

private:
	static std::unordered_map<ImGuiFontType, uint32_t> s_ImGuiFontsIndexMap;
		
	//Used to create a UI context
	static uint32_t s_contextID;
	//Used to generate unique IDs for each UI item in a context
	static uint32_t s_itemCounter;
	//Used as the ID for each UI item
	static char s_itemID[16];
};

template<typename TEnum>
bool ImGuiUtilities::property(const std::string_view label, const std::span<const std::string_view> options, TEnum& selected)
{
	bool modified = false;

	pushFont(ImGuiFontType::FONT_BOLD);
	ImGui::Text(label.data());
	popFont();

	ImGui::NextColumn();

	ImGui::PushItemWidth(-1.0f);

	const uint32_t indexOfSelectedOption = static_cast<uint32_t>(selected);
	std::string_view currentOption = options[indexOfSelectedOption];

	if (ImGui::BeginCombo(generateItemID(), currentOption.data(), ImGuiComboFlags_NoArrowButton))
	{
		for (uint32_t i = 0; i < options.size(); i++)
		{
			bool isSelected = i == indexOfSelectedOption;
			if (ImGui::Selectable(options[i].data(), isSelected))
			{
				currentOption = options[i];
				selected = static_cast<TEnum>(i);
				modified = true;
			}

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (isSelected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}

	ImGui::PopItemWidth();

	ImGui::NextColumn();

	return modified;
}

template<typename StrType>
bool ImGuiUtilities::property(const std::string_view label, const std::span<const StrType> options, StrType& selected)
{
	bool modified = false;

	pushFont(ImGuiFontType::FONT_BOLD);
	ImGui::Text(label.data());
	popFont();

	ImGui::NextColumn();

	ImGui::PushItemWidth(-1.0f);

	if (ImGui::BeginCombo(generateItemID(), selected.data(), ImGuiComboFlags_NoArrowButton))
	{
		for (const auto& option : options)
		{
			bool isSelected = option == selected;
			if (ImGui::Selectable(option.data(), isSelected))
			{
				selected = option;
				modified = true;
			}

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (isSelected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}

	ImGui::PopItemWidth();

	ImGui::NextColumn();

	return modified;
}

}