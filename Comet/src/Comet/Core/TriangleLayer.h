#pragma once
#include "CometPCH.h"

#include "Layer.h"
#include "Comet/Renderer/Pipeline.h"
#include "Comet/Renderer/VertexBuffer.h"
#include "Comet/Renderer/ShaderDataType.h"
#include "Comet/Renderer/IndexBuffer.h"
#include "Comet/Renderer/Shader.h"
#include "Comet/Renderer/Texture.h"
#include "Comet/Renderer/Framebuffer.h"

#include "Comet/Renderer/Renderer.h"
#include "Comet/Renderer/Renderer2D.h"

#include "glm/gtc/matrix_transform.hpp"

#include "imgui.h"

#include "glad/glad.h"

namespace Comet
{

	class TriangleLayer : public Layer
	{
	public:
		TriangleLayer() : Layer("TriangleLayer"), m_ts(0.0f) {}

		void onAttach() override
		{
			Renderer::setClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));

			m_camera = Camera();

			/*float triangleVertexData[] = {
				 0.0f,  0.5f,  0.0f, 0.5f, 1.0f,
				-0.5f, -0.5f,  0.0f, 0.0f, 0.0f,
				 0.5f, -0.5f,  0.0f, 1.0f, 0.0f
			};
			m_vb = VertexBuffer::create(triangleVertexData, sizeof(triangleVertexData));

			PipelineSpecification spec;
			spec.layout = {
				{"a_Position", ShaderDataType::FLOAT3},
				{"a_TextureCoordinates", ShaderDataType::FLOAT2}
			};
			m_pipeline = Pipeline::create(spec);

			uint32_t indices[] = {
				0, 1, 2
			};
			m_ib = IndexBuffer::create(indices, 3);

			m_shader = Comet::Shader::create("assets/shaders/test.glsl");
			m_color = { 0.1f, 0.8f, 0.8f, 1.0f };
			m_x = 0;

			m_texture = Texture2D::create("assets/textures/container2.png");
			m_cubeMap = TextureCube::create("assets/cubeMaps/skyBox.jpg");

			FramebufferSpecification framebufferSpecification;
			framebufferSpecification.samples = 8;
			m_framebuffer = Framebuffer::create(framebufferSpecification);*/
		}
		void onDetach() override {}
		void onUpdate(Timestep ts) override 
		{
			/*m_vb->bind();
			m_pipeline->bind();
			m_ib->bind();
			m_texture->bind();*/

			m_ts = ts;

			float r = (glm::sin(m_x) + 1) * 100 * ts;
			float g = (glm::sin(m_x + 0.5) + 1) * 100 * ts;
			float b = (glm::sin(m_x + 1.0) + 1) * 100 * ts;
			m_color = { r, g, b, 1.0f };
			//m_shader->bind();
			//m_shader->setUniformData("ubo.color", m_color);
			m_x += 0.5f * ts;

			glm::mat4 cameraTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, m_x * 0.1f, 0.0f));
		}

		void onImGuiRender() override
		{
			ImGui::Begin("Test");

			ImGui::Text("Frametime: %fms", m_ts.getMilliseconds());

			ImGui::End();
		}

	private:
		Reference<Pipeline> m_pipeline;
		Reference<VertexBuffer> m_vb;
		Reference<IndexBuffer> m_ib;
		Reference<Shader> m_shader;
		Reference<Texture2D> m_texture;
		Reference<TextureCube> m_cubeMap;
		Reference<Framebuffer> m_framebuffer;
		glm::vec4 m_color;
		float m_x = 0;
		Camera m_camera;
		Timestep m_ts;
	};

}