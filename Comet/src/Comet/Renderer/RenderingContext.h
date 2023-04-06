#pragma once
#include "CometPCH.h"

namespace Comet
{

class RenderingContext
{
public:
	virtual ~RenderingContext() = default;

	static Unique<RenderingContext> create(void* const implementationWindowHandle);

	virtual void swapBuffers() const = 0;
	virtual void onResize(const uint32_t width, const uint32_t height) const = 0;
};

}