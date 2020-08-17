#pragma once
#include "CometPCH.h"

#include "Comet/Events/Event.h"

namespace Comet
{

	class Layer
	{
	public:
		Layer(const std::string& name = "Layer") : m_name(name) {}
		virtual ~Layer() = default;

		virtual void onAttach() = 0;
		virtual void onDetach() = 0;
		virtual void onUpdate() = 0;
		virtual void onEvent(Event& e) = 0;

		const std::string& getName() const { return m_name; }

	protected:
		std::string m_name;
	};

}