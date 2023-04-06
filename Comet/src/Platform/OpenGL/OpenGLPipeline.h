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

	void bind() override;

	const Specification& getPipelineSpecification() const override { return m_spec; }

private:
	Specification m_spec;
	RendererID m_vertexArrayRendererID = 0;
};

}