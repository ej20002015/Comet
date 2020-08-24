#pragma once
#include "CometPCH.h"

namespace Comet
{

	class RenderingContext
	{
	public:
		virtual ~RenderingContext() = default;

		static Unique<RenderingContext> create(void* implementationWindowHandle);

		virtual void swapBuffers() = 0;
	};

}