#pragma once
#include "CometPCH.h"

#include "VertexBufferLayout.h"

namespace Comet
{

class Pipeline
{
public:

	struct Specification
	{
		VertexBufferLayout layout;
	};

public:
	virtual ~Pipeline() = default;

	static Reference<Pipeline> create(const Specification& spec);

	//TODO: Remove when render command buffers are eventually added
	virtual void bind() = 0;

	virtual const Specification& getPipelineSpecification() const = 0;
};

}