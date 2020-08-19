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

		virtual void onAttach() {}
		virtual void onDetach() {}
		virtual void onUpdate() {}
		//Each layer can define ImGui logic within this method without having to start a frame or call render code - allows all layers to render ui on the application ImGuiLayer
		virtual void onImGuiRender() {}
		virtual void onEvent(Event& e) {}

		const std::string& getName() const { return m_name; }

	protected:
		std::string m_name;
	};

}