#pragma once
#include "CometPCH.h"

#include "Comet/Renderer/Pipeline.h"
#include "Comet/Renderer/RendererAPI.h"

namespace Comet
{

	class OpenGLPipeline : public Pipeline
	{
	public:
		OpenGLPipeline(const Specification& spec);
		~OpenGLPipeline() override;

		virtual void bind() override;

		virtual const Specification& getPipelineSpecification() const override { return m_spec; }

	private:
		Specification m_spec;
		RendererID m_vertexArrayRendererID = 0;
	};

}