#pragma once
#include "CometPCH.h"

#include "ImGui.h"

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
		//Window Size
		static ImVec2 getMinimumWindowSize();
		static void setMinimumWindowSize(const ImVec2& size);

		//Font Management
		static void loadFont(const std::string& filepath, float fontSize, ImGuiFontType fontType);
		static void setDefaultFont(ImGuiFontType fontType);

		static ImFont* getFont(ImGuiFontType fontType);

		static void pushFont(ImGuiFontType fontType);
		static void popFont();

	private:
		static std::unordered_map<ImGuiFontType, uint32_t> s_ImGuiFontsIndexMap;
	};

}