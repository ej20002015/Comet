#include "CometPCH.h"
#include "ImGuiUtilities.h"

#include "glm/gtc/type_ptr.hpp"

namespace Comet
{

    std::unordered_map<ImGuiUtilities::ImGuiFontType, uint32_t> ImGuiUtilities::s_ImGuiFontsIndexMap;
    uint32_t ImGuiUtilities::s_contextID = 0;
    uint32_t ImGuiUtilities::s_itemCounter = 0;
    char ImGuiUtilities::s_itemID[16];

    ImVec2 ImGuiUtilities::getMinimumWindowSize()
    {
        return ImGui::GetStyle().WindowMinSize;
    }

    void ImGuiUtilities::setMinimumWindowSize(const ImVec2& size)
    {
        ImGuiStyle& style = ImGui::GetStyle();
        style.WindowMinSize = size;
    }

    ImVec2 ImGuiUtilities::getWindowPadding()
    {
        return ImGui::GetStyle().WindowPadding;
    }

    void ImGuiUtilities::setWindowPadding(const ImVec2& padding)
    {
        ImGuiStyle& style = ImGui::GetStyle();
        style.WindowPadding = padding;
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

    void ImGuiUtilities::pushAlphaFactorStyleVariable(float alphaFactor)
    {
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * alphaFactor);
    }

    void ImGuiUtilities::popStyleVariable()
    {
        ImGui::PopStyleVar();
    }

    void ImGuiUtilities::seperator()
    {
        ImGui::Separator();
    }

    void ImGuiUtilities::pushID()
    {
        ImGui::PushID(s_contextID++);
    }

    void ImGuiUtilities::popID()
    {
        ImGui::PopID();
        //ID with s_contextID - 1 is no longer on the stack so can be used again
        s_contextID--;
    }

    void ImGuiUtilities::beginPropertyGrid()
    {
        //Create UI context for property grid by pushing an ID
        pushID();
        s_itemCounter = 0;
        //One column for the labels, another for the input fields
        ImGui::Columns(2, nullptr, false);
    }

    void ImGuiUtilities::endPropertyGrid()
    {
        ImGui::Columns(1);
        popID();
    }

    void ImGuiUtilities::property(const char* label, const std::string& value)
    {
        pushFont(ImGuiFontType::FONT_BOLD);
        ImGui::Text(label);
        popFont();

        ImGui::NextColumn();

        ImGui::PushItemWidth(-1.0f);
        pushAlphaFactorStyleVariable(0.5f);
        ImGui::InputText(generateItemID(), const_cast<char*>(value.c_str()), value.size() + 1, ImGuiInputTextFlags_ReadOnly);
        popStyleVariable();
        ImGui::PopItemWidth();

        ImGui::NextColumn();
    }

    bool ImGuiUtilities::property(const char* label, std::string& value)
    {
        bool modified = false;

        pushFont(ImGuiFontType::FONT_BOLD);
        ImGui::Text(label);
        popFont();

        ImGui::NextColumn();

        //Copy string into local buffer so it can be modified
        char textBuffer[256];
        strcpy_s(textBuffer, sizeof(textBuffer), value.c_str());

        ImGui::PushItemWidth(-1.0f);
        if (ImGui::InputText(generateItemID(), textBuffer, sizeof(textBuffer)))
        {
            value = std::string(textBuffer);
            modified = true;
        }
        ImGui::PopItemWidth();

        ImGui::NextColumn();

        return modified;
    }

    bool ImGuiUtilities::property(const char* label, int32_t& value, float delta, const char* format, int32_t min, int32_t max)
    {
        bool modified = false;

        pushFont(ImGuiFontType::FONT_BOLD);
        ImGui::Text(label);
        popFont();

        ImGui::NextColumn();

        ImGui::PushItemWidth(-1.0f);
        if (ImGui::DragInt(generateItemID(), &value, delta, min, max, format))
            modified = true;
        ImGui::PopItemWidth();

        ImGui::NextColumn();

        return modified;
    }

    bool ImGuiUtilities::property(const char* label, float& value, float delta, const char* format, float min, float max)
    {
        bool modified = false;

        pushFont(ImGuiFontType::FONT_BOLD);
        ImGui::Text(label);
        popFont();

        ImGui::NextColumn();

        ImGui::PushItemWidth(-1.0f);
        if (ImGui::DragFloat(generateItemID(), &value, delta, min, max, format))
            modified = true;
        ImGui::PopItemWidth();

        ImGui::NextColumn();

        return modified;
    }

    bool ImGuiUtilities::property(const char* label, glm::vec2& value, float delta, const char* format, float min, float max)
    {
        bool modified = false;

        pushFont(ImGuiFontType::FONT_BOLD);
        ImGui::Text(label);
        popFont();

        ImGui::NextColumn();

        ImGui::PushItemWidth(-1.0f);
        if (ImGui::DragFloat2(generateItemID(), glm::value_ptr(value), delta, min, max, format))
            modified = true;
        ImGui::PopItemWidth();

        ImGui::NextColumn();

        return modified;
    }

    bool ImGuiUtilities::property(const char* label, glm::vec3& value, float delta, const char* format, float min, float max)
    {
        bool modified = false;

        pushFont(ImGuiFontType::FONT_BOLD);
        ImGui::Text(label);
        popFont();

        ImGui::NextColumn();

        ImGui::PushItemWidth(-1.0f);
        if (ImGui::DragFloat3(generateItemID(), glm::value_ptr(value), delta, min, max, format))
            modified = true;
        ImGui::PopItemWidth();

        ImGui::NextColumn();

        return modified;
    }

    bool ImGuiUtilities::propertyColorPicker(const char* label, glm::vec4& value)
    {
        bool modified = false;

        pushFont(ImGuiFontType::FONT_BOLD);
        ImGui::Text(label);
        popFont();

        ImGui::NextColumn();

        ImGui::PushItemWidth(-1.0f);
        if (ImGui::ColorEdit4(generateItemID(), glm::value_ptr(value)))
            modified = true;
        ImGui::PopItemWidth();

        ImGui::NextColumn();

        return modified;
    }

    bool ImGuiUtilities::propertyButton(const char* label, const char* buttonText)
    {
        bool pressed = false;

        pushFont(ImGuiFontType::FONT_BOLD);
        ImGui::Text(label);
        popFont();

        ImGui::NextColumn();

        ImGui::PushItemWidth(-1.0f);
        if (ImGui::Button(buttonText))
            pressed = true;
        ImGui::PopItemWidth();

        ImGui::NextColumn();

        return pressed;
    }

    bool ImGuiUtilities::propertyImageButton(const char* label, uint32_t textureRendererID, const glm::vec2& size)
    {
        //ImageButtonIDs are based of the rendererID so need to push an ID to prevent conflicts arising
        pushID();

        bool pressed = false;

        pushFont(ImGuiFontType::FONT_BOLD);
        ImGui::Text(label);
        popFont();

        ImGui::NextColumn();

        ImGui::PushItemWidth(-1.0f);
        if (ImGui::ImageButton(reinterpret_cast<void*>(static_cast<uint64_t>((textureRendererID))), { size.x, size.y }, { 0.0f, 1.0f }, { 1.0f, 0.0f }, 0))
            pressed = true;
        ImGui::PopItemWidth();

        ImGui::NextColumn();

        popID();

        return pressed;
    }

    bool ImGuiUtilities::property(const char* label, bool& value)
    {
        bool modified = false;

        pushFont(ImGuiFontType::FONT_BOLD);
        ImGui::Text(label);
        popFont();

        ImGui::NextColumn();

        ImGui::PushItemWidth(-1.0f);
        if (ImGui::Checkbox(generateItemID(), &value))
            modified = true;
        ImGui::PopItemWidth();

        ImGui::NextColumn();

        return modified;
    }

    char* ImGuiUtilities::generateItemID()
    {
        //Indicate to ImGui that this ID should not be used as a label
        s_itemID[0] = '#';
        s_itemID[1] = '#';
        //Set the rest of the char array to 0s (null terminator)
        memset(s_itemID + 2, 0, 14);
        //Use s_itemCounter to generate a unique ID for the current item within a UI context
        std::snprintf(s_itemID + 2, 14, "%d", s_itemCounter++);

        //Return the buffer for convenience
        return s_itemID;
    }

}
