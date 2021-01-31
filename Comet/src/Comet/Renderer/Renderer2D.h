#pragma once
#include "CometPCH.h"

#include "Camera.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Pipeline.h"
#include "Shader.h"
#include "Texture.h"

namespace Comet
{

	class Renderer2D
	{
	public:

		struct Statistics
		{
			uint32_t quads = 0;
			uint32_t drawCalls = 0;
			uint32_t getVertices() const { return quads * 4; }
			uint32_t getTriangles() const { return quads * 2; }
		};

		Renderer2D() = delete;

		static void init();
		static void shutdown();

		static void beginScene(const Camera& camera, const glm::mat4& cameraTransform, bool depthTest = true);

		static void endScene();

		static void drawColoredQuad(const glm::vec2& centerCoordinates, const glm::vec2& scale = glm::vec3(1.0f), const glm::vec4& color = glm::vec4(1.0f), float tilingFactor = 1.0f);
		static void drawColoredQuad(const glm::vec3& centerCoordinates, const glm::vec2& scale = glm::vec3(1.0f), const glm::vec4& color = glm::vec4(1.0f), float tilingFactor = 1.0f);
		static void drawRotatedColoredQuad(const glm::vec2& centerCoordinates, float radians, const glm::vec2& scale = glm::vec3(1.0f), const glm::vec4& color = glm::vec4(1.0f), float tilingFactor = 1.0f);
		static void drawRotatedColoredQuad(const glm::vec3& centerCoordinates, float radians, const glm::vec2& scale = glm::vec3(1.0f), const glm::vec4& color = glm::vec4(1.0f), float tilingFactor = 1.0f);

		static void drawTexturedQuad(const glm::vec2& centerCoordinates, const Reference<Texture2D>& texture, const glm::vec2& scale = glm::vec3(1.0f), const glm::vec4& colorTint = glm::vec4(1.0f), float tilingFactor = 1.0f);
		static void drawTexturedQuad(const glm::vec3& centerCoordinates, const Reference<Texture2D>& texture, const glm::vec2& scale = glm::vec3(1.0f), const glm::vec4& colorTint = glm::vec4(1.0f), float tilingFactor = 1.0f);
		static void drawRotatedTexturedQuad(const glm::vec2& centerCoordinates, float radians, const Reference<Texture2D>& texture, const glm::vec2& scale = glm::vec3(1.0f), const glm::vec4& colorTint = glm::vec4(1.0f), float tilingFactor = 1.0f);
		static void drawRotatedTexturedQuad(const glm::vec3& centerCoordinates, float radians, const Reference<Texture2D>& texture, const glm::vec2& scale = glm::vec3(1.0f), const glm::vec4& colorTint = glm::vec4(1.0f), float tilingFactor = 1.0f);

		static void drawSubTexturedQuad(const glm::vec2& centerCoordinates, const Reference<Texture2DSubTexture>& texture, const glm::vec2& scale = glm::vec3(1.0f), const glm::vec4& colorTint = glm::vec4(1.0f), float tilingFactor = 1.0f);
		static void drawSubTexturedQuad(const glm::vec3& centerCoordinates, const Reference<Texture2DSubTexture>& subTexture, const glm::vec2& scale = glm::vec3(1.0f), const glm::vec4& colorTint = glm::vec4(1.0f), float tilingFactor = 1.0f);
		static void drawRotatedSubTexturedQuad(const glm::vec2& centerCoordinates, float radians, const Reference<Texture2DSubTexture>& subTexture, const glm::vec2& scale = glm::vec3(1.0f), const glm::vec4& colorTint = glm::vec4(1.0f), float tilingFactor = 1.0f);
		static void drawRotatedSubTexturedQuad(const glm::vec3& centerCoordinates, float radians, const Reference<Texture2DSubTexture>& subTexture, const glm::vec2& scale = glm::vec3(1.0f), const glm::vec4& colorTint = glm::vec4(1.0f), float tilingFactor = 1.0f);

		static void drawQuad(const glm::mat4& transform, const glm::vec4& color, const Reference<Texture2D>& texture, float tilingFactor = 1.0f);
		static void drawSubQuad(const glm::mat4& transform, const glm::vec4& color, const Reference<Texture2DSubTexture>& subTexture, float tilingFactor = 1.0f);

		static void resetStats();

		static Statistics getRenderer2DStats() { return s_stats; }
	private:
		static void setInitialBatchData();
		static void nextBatch();
		static void flush();

	private:

		static Statistics s_stats;

		struct QuadVertex
		{
			glm::vec3 position;
			glm::vec4 color;
			glm::vec2 textureCoordinates;
			//try an int
			float textureIndex;
			float tilingFactor;
		};

		//SET ON INITIALISATION OF RENDERER2D
		struct Renderer2DData
		{
			//Maximum values per draw call
			static const uint32_t maxQuads = 20000;
			static const uint32_t maxQuadVertices = maxQuads * 4;
			static const uint32_t maxQuadIndices = maxQuads * 6;
			static const uint32_t maxTextureSlots = 32;

			Reference<Pipeline> quadPipeline;
			Reference<VertexBuffer> quadVertexBuffer;
			Reference<IndexBuffer> quadIndexBuffer;

			Reference<Shader> quadShader;

			Reference<Texture2D> whiteTexture;

			std::array<Reference<Texture2D>, maxTextureSlots> textureSlots;

			glm::mat4 quadVertexPositions;
			glm::mat4x2 quadTextureCoordinates;

			QuadVertex* quadVertexBufferBase;
		};

		static Renderer2DData s_data;

		//DATA USED FOR EACH BATCH
		struct Renderer2DBatchData
		{
			uint32_t quadCount;
			QuadVertex* quadVertexBufferPointer;

			uint32_t textureSlotIndex;

			bool depthTest;

			glm::mat4 viewProjectionMatrix;
		};

		static Renderer2DBatchData s_batchData;

	};

}