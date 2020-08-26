#pragma once
#include "CometPCH.h"

#include "Layer.h"
#include "Comet/Renderer/Pipeline.h"
#include "Comet/Renderer/VertexBuffer.h"
#include "Comet/Renderer/IndexBuffer.h"

#include "glad/glad.h"

namespace Comet
{

	class TriangleLayer : public Layer
	{
	public:
		TriangleLayer() : Layer("TriangleLayer") {}

		void onAttach() override
		{
			RendererAPI::setClearColor(0.1f, 0.1f, 0.1f, 1.0f);

			float triangleVertexData[] = {
				 0.0f,  0.5f,  0.0f,
				-0.5f, -0.5f,  0.0f,
				 0.5f, -0.5f,  0.0f
			};
			m_vb = VertexBuffer::create(triangleVertexData, sizeof(triangleVertexData));

			PipelineSpecification spec;
			spec.layout = {
				{"a_Coordinates", VertexBufferDataType::Float3}
			};
			m_pipeline = Pipeline::create(spec);

			uint32_t indices[] = {
				0, 1, 2
			};
			m_ib = IndexBuffer::create(indices, 3);

		}
		void onDetach() override {}
		void onUpdate() override 
		{
			m_vb->bind();
			m_pipeline->bind();
			m_ib->bind();

			glDrawElements(GL_TRIANGLES, m_ib->getCount(), GL_UNSIGNED_INT, 0);
		}

	private:
		Reference<Pipeline> m_pipeline;
		Reference<VertexBuffer> m_vb;
		Reference<IndexBuffer> m_ib;
	};

}