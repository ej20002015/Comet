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

		void i_drawIndexed(uint32_t count, PrimitiveType primitive, bool depthTest = false) override;

		void i_setClearColor(const glm::vec4& color) override;
		void i_clear() override;

		bool i_getBackfaceCulling() const override;
		void i_setBackfaceCulling(bool culling) override;
	};

}