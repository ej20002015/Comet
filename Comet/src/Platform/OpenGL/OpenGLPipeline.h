#pragma once
#include "CometPCH.h"

#include "Comet/Renderer/Pipeline.h"
#include "Comet/Renderer/RendererAPI.h"

namespace Comet
{

	class OpenGLPipeline : public Pipeline
	{
	public:
		OpenGLPipeline(const PipelineSpecification& spec);
		~OpenGLPipeline() override;

		virtual void bind() override;

		virtual const PipelineSpecification& getPipelineSpecification() const override { return m_spec; }

	private:
		PipelineSpecification m_spec;
		RendererID m_vertexArrayRendererID = 0;
	};

}