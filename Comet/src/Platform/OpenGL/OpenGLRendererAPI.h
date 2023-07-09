#pragma once
#include "CometPCH.h"

#include "Comet/Renderer/RendererAPI.h"

namespace Comet
{

class OpenGLRendererAPI : public RendererAPI
{
protected:
	void i_init() override;
	void i_shutdown() override;

	void i_drawIndexed(const uint32_t count, const PrimitiveType primitive) override;
	void i_drawIndexedFromVertexOffset(uint32_t count, const void* startOfIndices, uint32_t vertexOffset) override;

	void i_setClearColor(const glm::vec4& color) override;
	void i_clear() override;

	bool i_getBackfaceCulling() const override;
	void i_setBackfaceCulling(const bool culling) override;

	bool i_getDepthTesting() const override;
	void i_setDepthTesting(const bool isDepthTestingEnabled) override;
};

}