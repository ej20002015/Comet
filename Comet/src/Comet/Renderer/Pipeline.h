#pragma once
#include "CometPCH.h"

#include "VertexBufferLayout.h"

namespace Comet
{

	struct PipelineSpecification
	{
		VertexBufferLayout layout;
	};

	class Pipeline
	{
	public:
		virtual ~Pipeline() = default;

		static Reference<Pipeline> create(const PipelineSpecification& spec);

		//TODO: Remove when render command buffers are eventually added
		virtual void bind() = 0;

		virtual const PipelineSpecification& getPipelineSpecification() const = 0;
	};

}