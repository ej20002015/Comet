#include "CometPCH.h"
#include "ImGuiUtilities.h"

namespace Comet
{

    std::unordered_map<ImGuiUtilities::ImGuiFontType, uint32_t> ImGuiUtilities::s_ImGuiFontsIndexMap;

    ImVec2 ImGuiUtilities::getMinimumWindowSize()
    {
        return ImGui::GetStyle().WindowMinSize;
    }

    void ImGuiUtilities::setMinimumWindowSize(const ImVec2& size)
    {
        ImGuiStyle& style = ImGui::GetStyle();
        style.WindowMinSize = size;
    }

    void ImGuiUtilities::loadFont(const std::string& filepath, float fontSize, ImGuiFontType fontType)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.Fonts->AddFontFromFileTTF(filepath.c_str(), fontSize);
        uint32_t index = io.Fonts->Fonts.size() - 1;
        s_ImGuiFontsIndexMap[fontType] = index;
    }

    void ImGuiUtilities::setDefaultFont(ImGuiFontType fontType)
    {
        ImFont* font = getFont(fontType);
        if (font)
        {
            ImGuiIO& io = ImGui::GetIO();
            io.FontDefault = font;
        }
    }

    ImFont* ImGuiUtilities::getFont(ImGuiFontType fontType)
    {
        if (auto it = s_ImGuiFontsIndexMap.find(fontType); it != s_ImGuiFontsIndexMap.end())
            return ImGui::GetIO().Fonts->Fonts[(*it).second];
        
        CMT_COMET_ASSERT_MESSAGE(false, "No font can be found of the supplied type");
        return nullptr;
    }

    void ImGuiUtilities::pushFont(ImGuiFontType fontType)
    {
        ImFont* font = getFont(fontType);
        if (font)
            ImGui::PushFont(font);
    }

    void ImGuiUtilities::popFont()
    {
        ImGui::PopFont();
    }

    void ImGuiUtilities::setDarkModeColors()
    {
        ImVec4* colors = ImGui::GetStyle().Colors;

        colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

        //Headers
        colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
        colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
        colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

        //Buttons
        colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
        colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
        colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

        //Frame Background
        colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
        colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
        colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

        //Tabs
        colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
        colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
        colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

        //Title Background
        colors[ImGuiCol_TitleBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };
        colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.95f, 0.1505f, 0.951f, 1.0f };
    }

}
