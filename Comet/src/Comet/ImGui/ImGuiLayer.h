#pragma once
#include "CometPCH.h"

#include "Comet/Core/Layer.h"
#include "Comet/Events/Event.h"
#include "Comet/Events/ApplicationEvent.h"
#include "Comet/Events/KeyboardEvent.h"
#include "Comet/Events/MouseEvent.h"

namespace Comet
{

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer() : Layer("ImGuiLayer") {}
		~ImGuiLayer() override;

		void onAttach() override;
		void onDetach() override;
		void onEvent(Event& e) override;

		void onImGuiRender() override;

		//Called to set up ImGui to render a frame
		void begin();
		//Called to render ImGui
		void end();

	private:
		float m_time = 0.0f;
	};

}