#pragma once

#include <Comet.h>

#include "imgui.h"

class ExampleLayer : public Comet::Layer
{
public:
	ExampleLayer() : Layer("ExampleLayer") {}

	void onAttach() override 
	{
		Comet::RendererAPI::setClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	}
	void onDetach() override {}
	void onUpdate() override {}
	//void onEvent(Comet::Event& e) override { Comet::Log::clientInfo("{0} event: {1}", m_name, e); }

	void onImGuiRender() override
	{
		ImGui::Begin("Test");

		ImGui::Text("Renderer Capabilites");

		ImGui::Separator();

		ImGui::Text("Vendor: %s", Comet::RendererAPI::getCapabilities().vendor.c_str());
		ImGui::Text("Renderer: %s", Comet::RendererAPI::getCapabilities().renderer.c_str());
		ImGui::Text("Version: %s", Comet::RendererAPI::getCapabilities().version.c_str());

		ImGui::Text("");

		ImGui::Text("Max Samples: %d", Comet::RendererAPI::getCapabilities().maxSamples);
		ImGui::Text("Max Anisotropy: %f", Comet::RendererAPI::getCapabilities().maxAnisotropy);
		ImGui::Text("Max Texture Units: %d", Comet::RendererAPI::getCapabilities().maxTextureUnits);

		ImGui::End();
	}
};
