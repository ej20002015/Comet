#include "CometPCH.h"
#include "Renderer2D.h"

#include "RendererAPI.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/quaternion.hpp"

namespace Comet
{
	Renderer2D::Statistics Renderer2D::s_stats;
	Renderer2D::Renderer2DData Renderer2D::s_data;
	Renderer2D::Renderer2DBatchData Renderer2D::s_batchData;

	void Renderer2D::init()
	{
		PipelineSpecification quadPipelineSpecification;
		quadPipelineSpecification.layout =
		{
			{ "a_position",          ShaderDataType::FLOAT3 },
			{ "a_color",             ShaderDataType::FLOAT4 },
			{ "a_textureCoordinates", ShaderDataType::FLOAT2 },
			{ "a_textureIndex",      ShaderDataType::FLOAT },
			{ "a_tilingFactor",      ShaderDataType::FLOAT }
		};

		s_data.quadPipeline = Pipeline::create(quadPipelineSpecification);

		s_data.quadVertexBuffer = VertexBuffer::create(s_data.maxQuadVertices * sizeof(QuadVertex));
		s_data.quadVertexBufferBase = new QuadVertex[s_data.maxQuadVertices];

		uint32_t* quadIndices = new uint32_t[s_data.maxQuadIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_data.maxQuadIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		s_data.quadIndexBuffer = IndexBuffer::create(quadIndices, s_data.maxQuadIndices);
		delete[] quadIndices;

		s_data.quadShader = Shader::create("assets/shaders/2DBatchShader.glsl");

		s_data.whiteTexture = Texture2D::create(TextureFormat::RGBA, 1, 1);
		uint32_t whitePixelData = 0xffffffff;
		s_data.whiteTexture->setData(&whitePixelData, sizeof(whitePixelData));

		s_data.textureSlots[0] = s_data.whiteTexture;

		//From bottom left counter clockwise
		s_data.quadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_data.quadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		s_data.quadVertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
		s_data.quadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

		s_data.quadTextureCoordinates[0] = { 0.0f, 0.0f };
		s_data.quadTextureCoordinates[1] = { 1.0f, 0.0f };
		s_data.quadTextureCoordinates[2] = { 1.0f, 1.0f };
		s_data.quadTextureCoordinates[3] = { 0.0f, 1.0f };
	}

	void Renderer2D::shutdown()
	{
		delete[] s_data.quadVertexBufferBase;
	}

	void Renderer2D::beginScene(const Camera& camera, const glm::mat4& viewMatrix, bool depthTest)
	{
		s_batchData.depthTest = depthTest;
		s_batchData.viewProjectionMatrix = camera.getProjectionMatrix() * viewMatrix;
		setInitialBatchData();
		resetStats();
	}

	void Renderer2D::endScene()
	{
		flush();
	}

	void Renderer2D::drawColoredQuad(const glm::vec2& centerCoordinates, const glm::vec2& scale, const glm::vec4& color, float tilingFactor)
	{
		drawColoredQuad({ centerCoordinates.x, centerCoordinates.y, 0.0f }, scale, color, tilingFactor);
	}

	void Renderer2D::drawColoredQuad(const glm::vec3& centerCoordinates, const glm::vec2& scale, const glm::vec4& color, float tilingFactor)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), centerCoordinates) * glm::scale(glm::mat4(1.0f), { scale.x, scale.y, 1.0f });
		drawQuad(transform, color, nullptr, tilingFactor);
	}

	void Renderer2D::drawRotatedColoredQuad(const glm::vec2& centerCoordinates, float radians, const glm::vec2& scale, const glm::vec4& color, float tilingFactor)
	{
		drawRotatedColoredQuad({ centerCoordinates.x, centerCoordinates.y, 0.0f }, radians, scale, color, tilingFactor);
	}

	void Renderer2D::drawRotatedColoredQuad(const glm::vec3& centerCoordinates, float radians, const glm::vec2& scale, const glm::vec4& color, float tilingFactor)
	{
		glm::quat rotation = glm::angleAxis(radians, glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), centerCoordinates) * glm::toMat4(rotation) * glm::scale(glm::mat4(1.0f), { scale.x, scale.y, 1.0f });
		drawQuad(transform, color, nullptr, tilingFactor);
	}

	void Renderer2D::drawTexturedQuad(const glm::vec2& centerCoordinates, Reference<Texture2D> texture, const glm::vec2& scale, const glm::vec4& colorTint, float tilingFactor)
	{
		drawTexturedQuad({ centerCoordinates.x, centerCoordinates.y, 0.0f }, texture, scale, colorTint, tilingFactor);
	}

	void Renderer2D::drawTexturedQuad(const glm::vec3& centerCoordinates, Reference<Texture2D> texture, const glm::vec2& scale, const glm::vec4& colorTint, float tilingFactor)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), centerCoordinates) * glm::scale(glm::mat4(1.0f), { scale.x, scale.y, 1.0f });
		drawQuad(transform, colorTint, texture, tilingFactor);
	}

	void Renderer2D::drawRotatedTexturedQuad(const glm::vec2& centerCoordinates, float radians, Reference<Texture2D> texture, const glm::vec2& scale, const glm::vec4& colorTint, float tilingFactor)
	{
		drawRotatedTexturedQuad({ centerCoordinates.x, centerCoordinates.y, 0.0f }, radians, texture, scale, colorTint, tilingFactor);
	}

	void Renderer2D::drawRotatedTexturedQuad(const glm::vec3& centerCoordinates, float radians, Reference<Texture2D> texture, const glm::vec2& scale, const glm::vec4& colorTint, float tilingFactor)
	{
		glm::quat rotation = glm::angleAxis(radians, glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), centerCoordinates) * glm::toMat4(rotation) * glm::scale(glm::mat4(1.0f), { scale.x, scale.y, 1.0f });
		drawQuad(transform, colorTint, texture, tilingFactor);
	}

	void Renderer2D::drawQuad(const glm::mat4& transform, const glm::vec4& color, Reference<Texture2D> texture, float tilingFactor)
	{
		//Check to see if max quads per batch has been exceeded
		if (s_batchData.quadCount >= s_data.maxQuads)
			nextBatch();

		glm::mat4 vertexPositions = transform * s_data.quadVertexPositions;

		float textureIndex = 0.0f;
		
		if (texture)
		{
			for (uint32_t i = 0; i < s_batchData.textureSlotIndex; ++i)
			{
				if (*texture == *(s_data.textureSlots[i]))
					textureIndex = static_cast<float>(i);
			}

			if (textureIndex == 0.0f)
			{
				if (s_batchData.textureSlotIndex == s_data.maxTextureSlots - 1)
					nextBatch();

				textureIndex = static_cast<float>(s_batchData.textureSlotIndex);
				s_data.textureSlots[s_batchData.textureSlotIndex] = texture;
				s_batchData.textureSlotIndex++;
			}
		}

		for (uint32_t i = 0; i < 4; ++i)
		{
			s_batchData.quadVertexBufferPointer->position = vertexPositions[i];
			s_batchData.quadVertexBufferPointer->color = color;
			s_batchData.quadVertexBufferPointer->textureCoordinates = s_data.quadTextureCoordinates[i];
			s_batchData.quadVertexBufferPointer->textureIndex = textureIndex;
			s_batchData.quadVertexBufferPointer->tilingFactor = tilingFactor;
			s_batchData.quadVertexBufferPointer++;
		}

		s_batchData.quadCount++;
		s_stats.quads++;
	}

	void Renderer2D::resetStats()
	{
		memset(&s_stats, 0, sizeof(Statistics));
	}

	void Renderer2D::setInitialBatchData()
	{
		s_batchData.quadCount = 0;
		s_batchData.textureSlotIndex = 1;
		s_batchData.quadVertexBufferPointer = s_data.quadVertexBufferBase;
	}

	void Renderer2D::nextBatch()
	{
		flush();
		setInitialBatchData();
	}

	void Renderer2D::flush()
	{
		if (!s_batchData.quadCount)
			return;

		//Only upload the populated area of the batch buffer to the vertex buffer
		uint32_t size = static_cast<uint32_t>((reinterpret_cast<uint8_t*>(s_batchData.quadVertexBufferPointer) - reinterpret_cast<uint8_t*>(s_data.quadVertexBufferBase)));
		s_data.quadVertexBuffer->setData(s_data.quadVertexBufferBase, size);

		//Bind
		for (uint32_t i = 0; i < s_batchData.textureSlotIndex; ++i)
			s_data.textureSlots[i]->bind(i);

		s_data.quadVertexBuffer->bind();
		s_data.quadPipeline->bind();
		s_data.quadIndexBuffer->bind();

		s_data.quadShader->bind();
		s_data.quadShader->setUniformBuffer(0, &s_batchData.viewProjectionMatrix[0][0]);

		glm::mat4 viewProj = s_batchData.viewProjectionMatrix;

		glm::vec4 temp = viewProj * glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);

		//Draw call
		RendererAPI::drawIndexed(s_batchData.quadCount * 6, PrimitiveType::TRIANGLES, s_batchData.depthTest);

		s_stats.drawCalls++;
	}

}
