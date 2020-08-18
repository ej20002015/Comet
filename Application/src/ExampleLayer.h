#pragma once

#include <Comet.h>

class ExampleLayer : public Comet::Layer
{
public:
	ExampleLayer() : Layer("ExampleLayer") {}

	void onAttach() override {  }
	void onDetach() override {  }
	void onUpdate() override 
	{
		if (Comet::Input::isMouseButtonPressed(Comet::MouseCode::MOUSE_BUTTON_1))
		{
			Comet::Log::clientTrace("Hello");
		}
	}
	void onEvent(Comet::Event& e) override { Comet::Log::clientInfo("{0} event: {1}", m_name, e); }
};
