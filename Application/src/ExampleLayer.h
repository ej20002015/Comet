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

		
		m_textureAtlas = Comet::Texture2DAtlas::create("assets/textures/RPGpack_sheet_2X.png", 128, false , Comet::TextureFilter::NEAREST, Comet::TextureFilter::NEAREST);

		m_subTextures["Door"] = Comet::Texture2DSubTexture::create(m_textureAtlas, { 14.0f, 0.0f }, { 1.0f, 1.0f });
		m_subTextures["BottomWall"] = Comet::Texture2DSubTexture::create(m_textureAtlas, { 0.0f, 9.0f }, { 1.0f, 1.0f });
		m_subTextures["LeftCornerWall"] = Comet::Texture2DSubTexture::create(m_textureAtlas, { 1.0f, 9.0f }, { 1.0f, 1.0f });
		m_subTextures["RightCornerWall"] = Comet::Texture2DSubTexture::create(m_textureAtlas, { 3.0f, 9.0f }, { 1.0f, 1.0f });
		m_subTextures["Wall"] = Comet::Texture2DSubTexture::create(m_textureAtlas, { 5.0f, 9.0f }, { 1.0f, 1.0f });
		m_subTextures["SmallWindow"] = Comet::Texture2DSubTexture::create(m_textureAtlas, { 12.0f, 0.0f }, { 1.0f, 1.0f });
		m_subTextures["LeftWall"] = Comet::Texture2DSubTexture::create(m_textureAtlas, { 4.0f, 9.0f }, { 1.0f, 1.0f });
		m_subTextures["RightWall"] = Comet::Texture2DSubTexture::create(m_textureAtlas, { 6.0f, 9.0f }, { 1.0f, 1.0f });
		m_subTextures["Roof"] = Comet::Texture2DSubTexture::create(m_textureAtlas, { 2.0f, 4.0f }, { 2.0f, 3.0f });
		m_subTextures["GrassDirtTop"] = Comet::Texture2DSubTexture::create(m_textureAtlas, { 1.0f, 12.0f }, { 1.0f, 1.0f });
		m_subTextures["GrassDirtBottom"] = Comet::Texture2DSubTexture::create(m_textureAtlas, { 1.0f, 10.0f }, { 1.0f, 1.0f });
		m_subTextures["GrassDirtLeft"] = Comet::Texture2DSubTexture::create(m_textureAtlas, { 0.0f, 11.0f }, { 1.0f, 1.0f });
		m_subTextures["GrassDirtRight"] = Comet::Texture2DSubTexture::create(m_textureAtlas, { 2.0f, 11.0f }, { 1.0f, 1.0f });
		m_subTextures["GrassDirtBottomLeft"] = Comet::Texture2DSubTexture::create(m_textureAtlas, { 0.0f, 10.0f }, { 1.0f, 1.0f });
		m_subTextures["GrassDirtBottomRight"] = Comet::Texture2DSubTexture::create(m_textureAtlas, { 2.0f, 10.0f }, { 1.0f, 1.0f });
		m_subTextures["GrassDirtTopRight"] = Comet::Texture2DSubTexture::create(m_textureAtlas, { 2.0f, 12.0f }, { 1.0f, 1.0f });
		m_subTextures["GrassDirtTopLeft"] = Comet::Texture2DSubTexture::create(m_textureAtlas, { 0.0f, 12.0f }, { 1.0f, 1.0f });
		m_subTextures["Grass"] = Comet::Texture2DSubTexture::create(m_textureAtlas, { 1.0f, 11.0f }, { 1.0f, 1.0f });
		m_subTextures["Tree"] = Comet::Texture2DSubTexture::create(m_textureAtlas, { 0.0f, 1.0f }, { 1.0f, 2.0f });


		m_orthographicCamera = Comet::OrthographicCamera(static_cast<float>(Comet::Application::get().getWindow().getWidth()) / static_cast<float>(Comet::Application::get().getWindow().getHeight()));
	}
	void onDetach() override {}
	void onUpdate(Comet::Timestep ts) override
	{
		m_ts = ts;

		m_orthographicCamera.onUpdate(ts);

		Comet::Renderer2D::beginScene(m_orthographicCamera, m_orthographicCamera.getViewMatrix(), true);

		/*const uint32_t quadCount = 10;
		const float stepSize = 2.0f / static_cast<float>(quadCount);


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
				Comet::Renderer2D::drawSubTexturedQuad({ x, y }, m_subTexture, glm::vec2(stepSize * 0.95f), color);
			}
		}*/

		Comet::Renderer2D::drawSubTexturedQuad({ 0.0f,  0.0f, -0.1f }, m_subTextures.at("BottomWall"));
		Comet::Renderer2D::drawSubTexturedQuad({  0.0f,  0.0f }, m_subTextures.at("Door"));
		Comet::Renderer2D::drawSubTexturedQuad({ -1.0f,  0.0f }, m_subTextures.at("LeftCornerWall"));
		Comet::Renderer2D::drawSubTexturedQuad({  1.0f,  0.0f }, m_subTextures.at("RightCornerWall"));
		Comet::Renderer2D::drawSubTexturedQuad({ 0.0f,  1.0f, -0.1f }, m_subTextures.at("Wall"));
		Comet::Renderer2D::drawSubTexturedQuad({  0.0f,  1.0f }, m_subTextures.at("SmallWindow"));
		Comet::Renderer2D::drawSubTexturedQuad({ -1.0f,  1.0f }, m_subTextures.at("LeftWall"));
		Comet::Renderer2D::drawSubTexturedQuad({  1.0f,  1.0f }, m_subTextures.at("RightWall"));
		Comet::Renderer2D::drawSubTexturedQuad({ -1.0f,  2.0f }, m_subTextures.at("Wall"));
		Comet::Renderer2D::drawSubTexturedQuad({  0.0f,  2.0f }, m_subTextures.at("Wall"));
		Comet::Renderer2D::drawSubTexturedQuad({  1.0f,  2.0f }, m_subTextures.at("Wall"));

		Comet::Renderer2D::drawSubTexturedQuad({ -2.0f,  -2.0f }, m_subTextures.at("GrassDirtBottomLeft"));
		Comet::Renderer2D::drawSubTexturedQuad({  2.0f,  -2.0f }, m_subTextures.at("GrassDirtBottomRight"));
		Comet::Renderer2D::drawSubTexturedQuad({ -1.0f,  -2.0f }, m_subTextures.at("GrassDirtBottom"));
		Comet::Renderer2D::drawSubTexturedQuad({  0.0f,  -2.0f }, m_subTextures.at("GrassDirtBottom"));
		Comet::Renderer2D::drawSubTexturedQuad({  1.0f,  -2.0f }, m_subTextures.at("GrassDirtBottom"));
		Comet::Renderer2D::drawSubTexturedQuad({ -1.0f,  -1.0f }, m_subTextures.at("GrassDirtTop"));
		Comet::Renderer2D::drawSubTexturedQuad({  0.0f,  -1.0f }, m_subTextures.at("GrassDirtTop"));
		Comet::Renderer2D::drawSubTexturedQuad({  1.0f,  -1.0f }, m_subTextures.at("GrassDirtTop"));
		Comet::Renderer2D::drawSubTexturedQuad({  2.0f,  -1.0f }, m_subTextures.at("GrassDirtRight"));
		Comet::Renderer2D::drawSubTexturedQuad({ -2.0f,  -1.0f }, m_subTextures.at("GrassDirtLeft"));
		Comet::Renderer2D::drawSubTexturedQuad({ -2.0f,   0.0f }, m_subTextures.at("GrassDirtLeft"));
		Comet::Renderer2D::drawSubTexturedQuad({  2.0f,   0.0f }, m_subTextures.at("GrassDirtRight"));
		Comet::Renderer2D::drawSubTexturedQuad({ -2.0f,   0.0f }, m_subTextures.at("GrassDirtLeft"));
		Comet::Renderer2D::drawSubTexturedQuad({  2.0f,   0.0f }, m_subTextures.at("GrassDirtRight"));
		Comet::Renderer2D::drawSubTexturedQuad({ -2.0f,   1.0f }, m_subTextures.at("GrassDirtLeft"));
		Comet::Renderer2D::drawSubTexturedQuad({  2.0f,   1.0f }, m_subTextures.at("GrassDirtRight"));
		Comet::Renderer2D::drawSubTexturedQuad({ -2.0f,   2.0f }, m_subTextures.at("GrassDirtLeft"));
		Comet::Renderer2D::drawSubTexturedQuad({  2.0f,   2.0f }, m_subTextures.at("GrassDirtRight"));
		Comet::Renderer2D::drawSubTexturedQuad({ -2.0f,   3.0f }, m_subTextures.at("GrassDirtLeft"));
		Comet::Renderer2D::drawSubTexturedQuad({  2.0f,   3.0f }, m_subTextures.at("GrassDirtRight"));
		Comet::Renderer2D::drawSubTexturedQuad({ -2.0f,   4.0f }, m_subTextures.at("GrassDirtLeft"));
		Comet::Renderer2D::drawSubTexturedQuad({  2.0f,   4.0f }, m_subTextures.at("GrassDirtRight"));
		Comet::Renderer2D::drawSubTexturedQuad({ -1.0f,   4.0f }, m_subTextures.at("Grass"));
		Comet::Renderer2D::drawSubTexturedQuad({  0.0f,   4.0f }, m_subTextures.at("Grass"));
		Comet::Renderer2D::drawSubTexturedQuad({  1.0f,   4.0f }, m_subTextures.at("Grass"));
		Comet::Renderer2D::drawSubTexturedQuad({ -2.0f,   5.0f }, m_subTextures.at("GrassDirtTopLeft"));
		Comet::Renderer2D::drawSubTexturedQuad({  2.0f,   5.0f }, m_subTextures.at("GrassDirtTopRight"));
		Comet::Renderer2D::drawSubTexturedQuad({ -1.0f,   5.0f }, m_subTextures.at("GrassDirtTop"));
		Comet::Renderer2D::drawSubTexturedQuad({  0.0f,   5.0f }, m_subTextures.at("GrassDirtTop"));
		Comet::Renderer2D::drawSubTexturedQuad({  1.0f,   5.0f }, m_subTextures.at("GrassDirtTop"));

		Comet::Renderer2D::drawSubTexturedQuad({ 0.0f,  3.0f, 0.1f }, m_subTextures.at("Roof"), { 3.0f, 3.0f });

		Comet::Renderer2D::drawSubTexturedQuad({ -2.0f, -1.0f, 0.1f }, m_subTextures.at("Tree"), { 1.0f, 2.0f });
		Comet::Renderer2D::drawSubTexturedQuad({  2.0f, -1.0f, 0.1f }, m_subTextures.at("Tree"), { 1.0f, 2.0f });

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
	Comet::Reference<Comet::Texture2DAtlas> m_textureAtlas;
	std::unordered_map<std::string, Comet::Reference<Comet::Texture2DSubTexture>> m_subTextures;
};
