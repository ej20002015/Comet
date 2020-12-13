#pragma once
#include "CometPCH.h"

#include "Framebuffer.h"

namespace Comet
{
	
	struct RenderPassSpecification
	{
		Reference<Framebuffer> targetFramebuffer;
	};

	class RenderPass
	{
	public:
		static Reference<RenderPass> create(const RenderPassSpecification& specification);
		virtual ~RenderPass() = default;

		virtual const RenderPassSpecification& getSpecification() const = 0;
	};

}