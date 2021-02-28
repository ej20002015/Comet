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
        {
            return ImGui::GetIO().Fonts->Fonts[(*it).second];
        }
        
        Log::cometError("No font can be found of the supplied type");
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

}
