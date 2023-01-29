#pragma once
#include "CometPCH.h"

#include "Framebuffer.h"

namespace Comet
{
	
class RenderPass
{
public:

	struct Specification
	{
		Reference<Framebuffer> targetFramebuffer;
	};

public:
	static Reference<RenderPass> create(const Specification& specification);
	virtual ~RenderPass() = default;

	virtual const Specification& getSpecification() const = 0;
};

}