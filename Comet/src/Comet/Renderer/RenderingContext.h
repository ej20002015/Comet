#pragma once
#include "CometPCH.h"

namespace Comet
{

	class RenderingContext
	{
	public:
		virtual ~RenderingContext() = default;

		static Unique<RenderingContext> create(void* implementationWindowHandle);

		virtual void swapBuffers() const = 0;
		virtual void onResize(uint32_t width, uint32_t height) const = 0;
	};

}