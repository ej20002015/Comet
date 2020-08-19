#pragma once

#include <Comet.h>

#include "imgui.h"

class ExampleLayer : public Comet::Layer
{
public:
	ExampleLayer() : Layer("ExampleLayer") {}

	void onAttach() override {}
	void onDetach() override {}
	void onUpdate() override {}
	void onEvent(Comet::Event& e) override { Comet::Log::clientInfo("{0} event: {1}", m_name, e); }

	void onImGuiRender() override
	{
		ImGui::Begin("Test");
		ImGui::Text("Hello World!");
		ImGui::End();
	}
};
