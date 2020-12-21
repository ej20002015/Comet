#include "CometPCH.h"
#include "Renderer2D.h"

#include "RendererAPI.h"

#include "VertexBufferLayout.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Pipeline.h"

namespace Comet
{

	Renderer2D::Renderer2DData Renderer2D::s_data = Renderer2D::Renderer2DData();

	void Renderer2D::init()
	{
		s_data.quadPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_data.quadPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
		s_data.quadPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
		s_data.quadPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

		s_data.shader = Shader::create("assets/shaders/2DBatchShader.glsl");

		s_data.whiteTexture = Texture2D::create(TextureFormat::RGBA, 1, 1);
		uint32_t whitePixel = 0xffffffff;
		s_data.whiteTexture->setData(&whitePixel, sizeof(uint32_t));
	}

	void Renderer2D::shutdown()
	{
	}

	void Renderer2D::beginScene(const Camera& camera, const glm::mat4& cameraTransform)
	{
		glm::mat4 viewProjectionMatrix = camera.getProjectionMatrix() * cameraTransform;
		s_data.shader->bind();
		s_data.shader->setUniformBuffer(0, &viewProjectionMatrix[0][0]);
	}

	void Renderer2D::endScene()
	{
	}

	void Renderer2D::drawQuad(const glm::mat4& transform, const glm::vec4& color)
	{
		VertexBufferLayout layout =
		{
			{ "a_position",          ShaderDataType::FLOAT3 },
			{ "a_color",             ShaderDataType::FLOAT4 },
			{ "a_textureCoordinate", ShaderDataType::FLOAT2 },
			{ "a_textureIndex",      ShaderDataType::FLOAT },
			{ "a_tilingFactor",      ShaderDataType::FLOAT }
		};

		glm::mat4 positions = transform * s_data.quadPositions;
		float vertexBufferData[] =
		{
			positions[0][0], positions[0][1], positions[0][2], color[0], color[1], color[2], color[3], 0.0f, 0.0f, 1.0f, 1.0f,
			positions[1][0], positions[1][1], positions[1][2], color[0], color[1], color[2], color[3], 1.0f, 0.0f, 1.0f, 1.0f,
			positions[2][0], positions[2][1], positions[2][2], color[0], color[1], color[2], color[3], 1.0f, 1.0f, 1.0f, 1.0f,
			positions[3][0], positions[3][1], positions[3][2], color[0], color[1], color[2], color[3], 0.0f, 1.0f, 1.0f, 1.0f
		};

		Reference<VertexBuffer> vb = VertexBuffer::create(vertexBufferData, sizeof(vertexBufferData));

		PipelineSpecification pipelineSpec;
		pipelineSpec.layout = layout;
		Reference<Pipeline> pipeline = Pipeline::create(pipelineSpec);

		uint32_t indices[] = {
				0, 1, 2,
				2, 3, 0
		};
		Reference<IndexBuffer> ib = IndexBuffer::create(indices, 6);

		vb->bind();
		pipeline->bind();
		ib->bind();
		s_data.whiteTexture->bind(1);

		RendererAPI::drawIndexed(6, PrimitiveType::TRIANGLES);
	}

}
