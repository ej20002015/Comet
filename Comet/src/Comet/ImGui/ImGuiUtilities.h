#pragma once
#include "CometPCH.h"

#include "ImGui.h"
#include "glm/glm.hpp"

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

		//Window Size
		static ImVec2 getMinimumWindowSize();
		static void setMinimumWindowSize(const ImVec2& size);

		//Window Padding
		static ImVec2 getWindowPadding();
		static void setWindowPadding(const ImVec2& padding);

		//Font Management
		static void loadFont(const std::string& filepath, float fontSize, ImGuiFontType fontType);
		static void setDefaultFont(ImGuiFontType fontType);

		static ImFont* getFont(ImGuiFontType fontType);

		static void pushFont(ImGuiFontType fontType);
		static void popFont();

		//Window Colours
		static void setDarkModeColors();

		//Style Vars
		static void pushAlphaFactorStyleVariable(float alphaFactor);
		static void popStyleVariable();

		//Helper Methods
		static void seperator();

		//Context IDs
		static void pushID();
		static void popID();

		//Property Grid
		static void beginPropertyGrid();
		static void endPropertyGrid();

		//Property Elements
		static void property(const char* label, const std::string& value);
		static bool property(const char* label, std::string& value);

		static bool property(const char* label, int32_t& value, float delta = 1.0f, const char* format = "%d", int32_t min = 0, int32_t max = 0);

		static bool property(const char* label, float& value, float delta = 0.1f, const char* format = "%.1f", float min = 0.0f, float max = 0.0f);
		static bool property(const char* label, glm::vec2& value, float delta = 0.1f, const char* format = "%.1f", float min = 0.0f, float max = 0.0f);
		static bool property(const char* label, glm::vec3& value, float delta = 0.1f, const char* format = "%.1f", float min = 0.0f, float max = 0.0f);

		static bool propertyColorPicker(const char* label, glm::vec4& value);

		static bool propertyButton(const char* label, const char* buttonText);
		static bool propertyImageButton(const char* label, uint32_t textureRendererID, const glm::vec2& size);

		static bool property(const char* label, bool& value);

		template<typename TEnum>
		static bool property(const char* label, const char** options, uint32_t optionsCount, TEnum& selected);

		//Used to create a UI context
		static uint32_t s_contextID;

	private:
		//Item IDs
		static char* generateItemID();

	private:
		static std::unordered_map<ImGuiFontType, uint32_t> s_ImGuiFontsIndexMap;
		
		//Used to generate unique IDs for each UI item in a context
		static uint32_t s_itemCounter;
		//Used as the ID for each UI item
		static char s_itemID[16];
	};

	template<typename TEnum>
	bool ImGuiUtilities::property(const char* label, const char** options, uint32_t optionsCount, TEnum& selected)
	{
		bool modified = false;

		pushFont(ImGuiFontType::FONT_BOLD);
		ImGui::Text(label);
		popFont();

		ImGui::NextColumn();

		ImGui::PushItemWidth(-1.0f);

		uint32_t indexOfSelectedOption = static_cast<uint32_t>(selected);
		const char* currentOption = options[indexOfSelectedOption];

		if (ImGui::BeginCombo(generateItemID(), currentOption, ImGuiComboFlags_NoArrowButton))
		{
			for (uint32_t i = 0; i < optionsCount; i++)
			{
				bool isSelected = i == indexOfSelectedOption;
				if (ImGui::Selectable(options[i], isSelected))
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

}