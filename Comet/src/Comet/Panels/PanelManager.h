#pragma once
#include "CometPCH.h"

#include "Panel.h"
#include "PanelRegistry.h"

namespace Comet
{

class PanelManager
{
public:
	PanelManager() = default;

	template<std::derived_from<Panel> T>
	void add()
	{
		CMT_COMET_ASSERT_MESSAGE(!has<T>(), "Panel Manager already has an instance of the given panel");
		m_panels.insert({ typeid(T), { PanelRegistry::createRegisteredPanel<T>(), true } });
	}

	template<std::derived_from<Panel> T>
	void remove()
	{
		CMT_COMET_ASSERT_MESSAGE(has<T>(), "Panel Manager doesn't have an instance of the given panel");
		m_panels.erase(typeid(T));
	}

	template<std::derived_from<Panel> T>
	bool has() const
	{
		return m_panels.find(typeid(T)) != m_panels.end();
	}

	template<std::derived_from<Panel> T>
	T& get()
	{
		CMT_COMET_ASSERT_MESSAGE(has<T>(), "Panel Manager doesn't have an instance of the given panel");
		return *std::static_pointer_cast<T>(m_panels[typeid(T)].panel);
	}

	template<std::derived_from<Panel> T>
	void setVisible()
	{
		CMT_COMET_ASSERT_MESSAGE(has<T>(), "Panel Manager doesn't have an instance of the given panel");
		m_panels[typeid(T)].isVisible = true;
	}

	template<std::derived_from<Panel> T>
	void setNotVisible()
	{
		CMT_COMET_ASSERT_MESSAGE(has<T>(), "Panel Manager doesn't have an instance of the given panel");
		m_panels[typeid(T)].isVisible = false;
	}

	void render();

private:

	struct ManagedPanel
	{
		Reference<Panel> panel;
		bool isVisible = true;
	};

private:
	std::unordered_map<std::type_index, ManagedPanel> m_panels;
};

}