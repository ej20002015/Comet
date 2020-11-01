#pragma once
#include "CometPCH.h"

#include "Layer.h"
#include "Comet/Renderer/Pipeline.h"
#include "Comet/Renderer/VertexBuffer.h"
#include "Comet/Renderer/ShaderDataType.h"
#include "Comet/Renderer/IndexBuffer.h"
#include "Comet/Renderer/Shader.h"
#include "Comet/Renderer/Texture.h"

#include "Comet/Renderer/Renderer.h"

#include "glad/glad.h"

namespace Comet
{

	class TriangleLayer : public Layer
	{
	public:
		TriangleLayer() : Layer("TriangleLayer") {}

		void onAttach() override
		{
			Renderer::setClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));

			float triangleVertexData[] = {
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
		}
		void onDetach() override {}
		void onUpdate() override 
		{
			m_vb->bind();
			m_pipeline->bind();
			m_ib->bind();
			m_texture->bind();

			float r = (glm::sin(m_x) + 1) * 0.5;
			float g = (glm::sin(m_x + 0.5) + 1) * 0.5;
			float b = (glm::sin(m_x + 1.0) + 1) * 0.5;
			m_color = { r, g, b, 1.0f };
			m_shader->bind();
			//m_shader->setUniformData("ubo.color", m_color);
			m_x += 0.005f;

			Renderer::drawIndexed(m_ib->getCount(), PrimitiveType::TRIANGLES);
		}

	private:
		Reference<Pipeline> m_pipeline;
		Reference<VertexBuffer> m_vb;
		Reference<IndexBuffer> m_ib;
		Reference<Shader> m_shader;
		Reference<Texture2D> m_texture;
		glm::vec4 m_color;
		float m_x;
	};

}