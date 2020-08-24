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

		void i_setClearColor(float r, float g, float b, float a) override;
	};

}