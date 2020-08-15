#pragma once

#include "CometPCH.h"
#include "Comet/Core/Layer.h"

namespace Comet
{

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer() : Layer("ImGuiLayer") {}
		~ImGuiLayer() override;

		void onAttach() override;
		void onDetach() override;
		void onUpdate() override;
		void onEvent(Event& e) override;

	private:
		float m_time = 0.0f;
	};

}