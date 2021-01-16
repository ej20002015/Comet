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
		virtual void onUpdate(Timestep ts) {}
		//Each layer can define ImGui logic within this method without having to start a frame or call render code - allows all layers to render ui on the application ImGuiLayer
		virtual void onImGuiRender() {}
		virtual void onEvent(Event& e) {}

		const std::string& getName() const { return m_name; }

		bool getBlocking() const { return m_blockEvents; }
		void setBlocking(bool block) { m_blockEvents = block; }

	private:
		std::string m_name;
		bool m_blockEvents = true;
	};

}