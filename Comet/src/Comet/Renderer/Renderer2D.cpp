#include "CometPCH.h"
#include "Renderer2D.h"

#include "RendererAPI.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/quaternion.hpp"

namespace Comet
{

Renderer2D::Statistics Renderer2D::s_stats;
Renderer2D::StaticData Renderer2D::s_data;
Renderer2D::BatchData Renderer2D::s_batchData;

void Renderer2D::init()
{
	Pipeline::Specification quadPipelineSpecification = {
		{
			{ "a_position",           ShaderDataType::FLOAT3 },
			{ "a_color",              ShaderDataType::FLOAT4 },
			{ "a_textureCoordinates", ShaderDataType::FLOAT2 },
			{ "a_textureIndex",       ShaderDataType::UINT },
			{ "a_tilingFactor",       ShaderDataType::FLOAT },
			{ "a_entityID",           ShaderDataType::INT }
		}
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

	uint32_t whitePixelData = 0xffffffff;
	s_data.whiteTexture = Texture2D::create(&whitePixelData, 4, Texture::Format::RGBA, 1, 1);

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

void Renderer2D::beginScene(const glm::mat4& viewProjectionMatrix, const bool depthTest)
{
	s_batchData.depthTest = depthTest;
	Shader::setUniformBuffer(0, &viewProjectionMatrix[0][0]);
	setInitialBatchData();
	resetStats();
}

void Renderer2D::beginScene(const Camera& camera, const glm::mat4& cameraTransform, const bool depthTest)
{
	beginScene(camera.getProjectionMatrix() * glm::inverse(cameraTransform), depthTest);
}

void Renderer2D::beginScene(const EditorCamera& editorCamera, const bool depthTest)
{
	beginScene(editorCamera.getViewProjectionMatrix(), depthTest);
}

void Renderer2D::endScene()
{
	flush();
}

void Renderer2D::drawQuad(const glm::mat4& transform, const glm::vec4& color, const Reference<Texture2D>& texture, const float tilingFactor, const int32_t entityID)
{
	//Check to see if max quads per batch has been exceeded
	if (s_batchData.quadCount >= s_data.maxQuads)
		nextBatch();

	glm::mat4 vertexPositions = transform * s_data.quadVertexPositions;

	uint32_t textureIndex = 0;
		
	if (texture)
	{
		for (uint32_t i = 0; i < s_batchData.textureSlotIndex; ++i)
		{
			if (*texture == *(s_data.textureSlots[i]))
				textureIndex = i;
		}

		if (!textureIndex)
		{
			if (s_batchData.textureSlotIndex == s_data.maxTextureSlots - 1)
				nextBatch();

			textureIndex = s_batchData.textureSlotIndex;
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
		s_batchData.quadVertexBufferPointer->entityID = entityID;
		s_batchData.quadVertexBufferPointer++;
	}

	s_batchData.quadCount++;
	s_stats.quads++;
}

void Renderer2D::drawSubQuad(const glm::mat4& transform, const glm::vec4& color, const Texture2DSubTexture& subTexture, const float tilingFactor, const int32_t entityID)
{
	//Check to see if max quads per batch has been exceeded
	if (s_batchData.quadCount >= s_data.maxQuads)
		nextBatch();

	glm::mat4 vertexPositions = transform * s_data.quadVertexPositions;

	uint32_t textureIndex = 0;

	if (subTexture.getTextureAtlas())
	{
		for (uint32_t i = 0; i < s_batchData.textureSlotIndex; ++i)
		{
			if (*(subTexture.getTextureAtlas()) == *(s_data.textureSlots[i]))
				textureIndex = i;
		}


		if (!textureIndex)
		{
			if (s_batchData.textureSlotIndex == s_data.maxTextureSlots - 1)
				nextBatch();

			textureIndex = s_batchData.textureSlotIndex;
			s_data.textureSlots[s_batchData.textureSlotIndex] = subTexture.getTextureAtlas();
			s_batchData.textureSlotIndex++;
		}
	}

	for (uint32_t i = 0; i < 4; ++i)
	{
		s_batchData.quadVertexBufferPointer->position = vertexPositions[i];
		s_batchData.quadVertexBufferPointer->color = color;
		s_batchData.quadVertexBufferPointer->textureCoordinates = subTexture.getTextureAtlasCoordinates()[i];
		s_batchData.quadVertexBufferPointer->textureIndex = textureIndex;
		s_batchData.quadVertexBufferPointer->tilingFactor = tilingFactor;
		s_batchData.quadVertexBufferPointer->entityID = entityID;
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

	if (RendererAPI::getDepthTesting() != s_batchData.depthTest)
		RendererAPI::setDepthTesting(s_batchData.depthTest);

	//Draw call
	RendererAPI::drawIndexed(s_batchData.quadCount * 6, RendererAPI::PrimitiveType::TRIANGLES);

	s_stats.drawCalls++;
}

}
