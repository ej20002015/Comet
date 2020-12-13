#pragma once
#include "Comet/Renderer/RenderPass.h"

namespace Comet
{

	class OpenGLRenderPass : public RenderPass
	{
	public:
		OpenGLRenderPass(const RenderPassSpecification& specification) : specification(specification) {}
		~OpenGLRenderPass() = default;

		const RenderPassSpecification& getSpecification() const override { return specification; }

	private:
		RenderPassSpecification specification;
	};

}