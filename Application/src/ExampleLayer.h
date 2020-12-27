#pragma once

#include <Comet.h>

#include "imgui.h"
#include "glm/gtc/matrix_transform.hpp"

#include "glm/glm.hpp"

class ExampleLayer : public Comet::Layer
{
public:
	ExampleLayer() : Layer("ExampleLayer"), m_orthographicCamera(1.0f) {}

	void onAttach() override
	{
		Comet::Renderer::setClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
		texture = Comet::Texture2D::create("assets/textures/container2.png");
		m_orthographicCamera = Comet::OrthographicCamera(static_cast<float>(Comet::Application::get().getWindow().getWidth()) / static_cast<float>(Comet::Application::get().getWindow().getHeight()));
	}
	void onDetach() override {}
	void onUpdate(Comet::Timestep ts) override
	{
		m_ts = ts;

		m_orthographicCamera.onUpdate(ts);

		const uint32_t quadCount = 10;
		const float stepSize = 2.0f / static_cast<float>(quadCount);

		Comet::Renderer2D::beginScene(m_orthographicCamera, m_orthographicCamera.getViewMatrix());

		for (uint32_t i = 0; i < quadCount; ++i)
		{
			float x = (static_cast<float>(i) * stepSize) - 1.0f + (stepSize / 2.0f);
			for (uint32_t j = 0; j < quadCount; ++j)
			{
				float y = - ((static_cast<float>(j) * stepSize) - 1.0f + (stepSize / 2.0f));
				glm::vec4 color;
				color.r = (glm::sin((x * quadCount) + y) + 1.0f) / 2.0f;
				color.g = (glm::sin((x * quadCount) + y + 0.9f) + 1.0f) / 2.0f;
				color.b = (glm::sin((x * quadCount) + y + 1.3f) + 1.0f) / 2.0f;
				color.a = 1.0f;
				Comet::Renderer2D::drawTexturedQuad({ x, y }, texture, glm::vec2(stepSize * 0.95f), color);
			}
		}

		Comet::Renderer2D::endScene();
	}
	//void onEvent(Comet::Event& e) override { Comet::Log::clientInfo("{0} event: {1}", m_name, e); }

	void onImGuiRender() override
	{
		ImGui::Begin("Rendering Device Information");

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

		ImGui::Begin("Renderer2D Statistics");

		ImGui::Text("Frametime");

		ImGui::Separator();

		ImGui::Text("%fms (%f.2 fps)", m_ts.getMilliseconds(), 1.0f / m_ts.getSeconds());

		ImGui::Separator();

		auto stats = Comet::Renderer2D::getRenderer2DStats();

		ImGui::Text("%u quads", stats.quads);
		ImGui::Text("%u vertices", stats.getVertices());
		ImGui::Text("%u triangles", stats.getTriangles());
		ImGui::Text("%u draw calls", stats.drawCalls);

		ImGui::End();
	}

	void onEvent(Comet::Event& e) override 
	{
		m_orthographicCamera.onEvent(e);
	}

private:
	Comet::Timestep m_ts;
	Comet::OrthographicCamera m_orthographicCamera;
	Comet::Reference<Comet::Texture2D> texture;
};
