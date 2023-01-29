#pragma once
#include "Comet/Renderer/RenderPass.h"

namespace Comet
{

	class OpenGLRenderPass : public RenderPass
	{
	public:
		OpenGLRenderPass(const Specification& specification) : specification(specification) {}
		~OpenGLRenderPass() = default;

		const Specification& getSpecification() const override { return specification; }

	private:
		const Specification specification;
	};

}