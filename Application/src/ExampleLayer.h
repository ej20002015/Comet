#pragma once

#include <Comet.h>

#include "imgui.h"

#include "glm/glm.hpp"

class ExampleLayer : public Comet::Layer
{
public:
	ExampleLayer() : Layer("ExampleLayer") {}

	void onAttach() override
	{
		Comet::RendererAPI::setClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
		m_testShader = Comet::Shader::create("assets/shaders/test.glsl");
		m_color = { 0.1f, 0.8f, 0.8f, 1.0f };
		x = 0;
	}
	void onDetach() override {}
	void onUpdate() override 
	{
		float r = (glm::sin(x) + 1) * 0.5;
		float g = (glm::sin(x + 0.5) + 1) * 0.5;
		float b = (glm::sin(x + 1.0) + 1) * 0.5;
		m_color = { r, g, b, 1.0f };
		m_testShader->bind();
		m_testShader->setUniformData("ubo.color", m_color);
		x += 0.005f;
	}
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

private:
	Comet::Reference<Comet::Shader> m_testShader;
	glm::vec4 m_color;
	float x;
};
