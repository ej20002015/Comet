#pragma once
#include "CometPCH.h"

#include "Camera.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Pipeline.h"
#include "Shader.h"
#include "Texture.h"
#include "EditorCamera.h"
#include "Framebuffer.h"

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

	static void beginScene(const glm::mat4& viewProjectionMatrix, const Reference<Framebuffer>& targetFramebuffer, const bool depthTest = false);
	static void beginScene(const Camera& camera, const glm::mat4& cameraTransform, const Reference<Framebuffer>& targetFramebuffer, const bool depthTest = false);
	static void beginScene(const EditorCamera& editorCamera, const Reference<Framebuffer>& targetFramebuffer, const bool depthTest = false);

	static void endScene();

	static void drawQuad(const glm::mat4& transform, const glm::vec4& color, const Reference<Texture2D>& texture, const float tilingFactor = 1.0f, const int32_t entityID = -1);
	static void drawSubQuad(const glm::mat4& transform, const glm::vec4& color, const Texture2DSubTexture& subTexture, const float tilingFactor = 1.0f, const int32_t entityID = -1);

	static void resetStats();

	static Statistics& getRenderer2DStats() { return s_stats; }
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
		uint32_t textureIndex;
		float tilingFactor;

		//For the editor only (TODO: decide how to separate this from the runtime renderer code execute at runtime)
		int32_t entityID;
	};

	//SET ON INITIALISATION OF RENDERER2D
	struct StaticData
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

	static StaticData s_data;

	//DATA USED FOR EACH BATCH
	struct BatchData
	{
		uint32_t quadCount;
		QuadVertex* quadVertexBufferPointer;

		uint32_t textureSlotIndex;

		Reference<Framebuffer> targetFramebuffer;

		bool depthTest;
	};

	static BatchData s_batchData;

};

}