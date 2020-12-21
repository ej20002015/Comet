#pragma once

#include <Comet.h>

#include "imgui.h"
#include "glm/gtc/matrix_transform.hpp"

#include "glm/glm.hpp"

class ExampleLayer : public Comet::Layer
{
public:
	ExampleLayer() : Layer("ExampleLayer") {}

	void onAttach() override
	{
		Comet::Renderer::setClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
	}
	void onDetach() override {}
	void onUpdate(Comet::Timestep ts) override 
	{
		m_ts = ts;

		Comet::Renderer2D::beginScene(m_camera, glm::mat4(1.0f));
		Comet::Renderer2D::drawQuad(glm::mat4(1.0f), { 1.0f, 0.0f, 0.0f, 0.5f });
		Comet::Renderer2D::drawQuad(glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, -0.5f, -0.1f)), glm::vec3(0.2f)), { 0.0f, 1.0f, 0.0f, 0.2f });
		Comet::Renderer2D::endScene();
	}
	//void onEvent(Comet::Event& e) override { Comet::Log::clientInfo("{0} event: {1}", m_name, e); }

	void onImGuiRender() override
	{
		ImGui::Begin("Information");

		ImGui::Text("Frametime");

		ImGui::Separator();

		ImGui::Text("%fms", m_ts.getMilliseconds());

		ImGui::Separator();

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
	Comet::Camera m_camera;
	Comet::Timestep m_ts;
};
