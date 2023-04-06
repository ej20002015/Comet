#include "WindowMenuPanel.h"

#include "imgui.h"

#include "Comet/Panels/PanelRegistry.h"

namespace Comet
{

REG_PANEL(WindowMenuBarPanel)

void WindowMenuBarPanel::setMenuEntries(const std::unordered_map<std::string, std::initializer_list<MenuItem>> menuEntries)
{
    m_menuEntries.clear();

    std::for_each(menuEntries.begin(), menuEntries.end(), [this](const auto& menuEntry)
    {
        m_menuEntries.insert({ menuEntry.first, menuEntry.second });
    });
}

void WindowMenuBarPanel::onImGuiRender()
{
    if (ImGui::BeginMenuBar())
    {
        for (const auto& [menuHeader, menuItems] : m_menuEntries)
        {
            if (ImGui::BeginMenu(menuHeader.c_str()))
            {
                for (const auto& menuItem : menuItems)
                {
                    const char* const shortcut = menuItem.shortcut ? menuItem.shortcut.value().c_str() : nullptr;
                    if (ImGui::MenuItem(menuItem.label.c_str(), shortcut))
                        menuItem.callback();
                }

                ImGui::EndMenu();
            }
        }

        ImGui::EndMenuBar();
    }
 
}

}
