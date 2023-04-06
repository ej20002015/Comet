#pragma once
#include "CometPCH.h"

#include "Comet/Panels/Panel.h"

namespace Comet
{

class WindowMenuBarPanel : public Panel
{
public:
	DEF_PANEL()

	struct MenuItem
	{
		std::string label;
		std::optional<std::string> shortcut;
		std::function<void(void)> callback;
	};

public:
	WindowMenuBarPanel(const std::unordered_map<std::string, std::initializer_list<MenuItem>> menuEntries) { setMenuEntries(menuEntries); }
	WindowMenuBarPanel() = default;

	void setMenuEntries(const std::unordered_map<std::string, std::initializer_list<MenuItem>> menuEntries);

	void onImGuiRender() override;

private:
	std::unordered_map<std::string, std::vector<MenuItem>> m_menuEntries;
};

}