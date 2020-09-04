#include "CometPCH.h"
#include "OpenGLIndexBuffer.h"

#include "Comet/Core/Buffer.h"

#include "glad/glad.h"

namespace Comet
{

	static GLenum getOpenGLUsage(IndexBufferUsage usage)
	{
		switch (usage)
		{
			case IndexBufferUsage::STATIC:    return GL_STATIC_DRAW; break;
			case IndexBufferUsage::DYNAMIC:   return GL_DYNAMIC_DRAW; break;
		}

		CMT_COMET_ASSERT_MESSAGE(false, "Unknown index buffer usage");
		return 0;
	}

	OpenGLIndexBuffer::OpenGLIndexBuffer(void* data, uint32_t count, IndexBufferUsage usage)
		: m_count(count), m_usage(usage)
	{
		m_localData = Buffer::create(data, count * sizeof(uint32_t));

		glCreateBuffers(1, &m_rendererID);
		glNamedBufferData(m_rendererID, count * sizeof(uint32_t), m_localData->getData(), getOpenGLUsage(usage));
	}

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t count, IndexBufferUsage usage)
		: m_localData(nullptr), m_count(count), m_usage(usage)
	{
		glCreateBuffers(1, &m_rendererID);
		glNamedBufferData(m_rendererID, count * sizeof(uint32_t), nullptr, getOpenGLUsage(usage));
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		glDeleteBuffers(1, &m_rendererID);
	}

	void OpenGLIndexBuffer::bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID);
	}

	void OpenGLIndexBuffer::setData(void* data, uint32_t count, uint32_t offset)
	{
		m_localData = Buffer::create(data, count * sizeof(uint32_t));
		m_count = count;

		glNamedBufferSubData(m_rendererID, offset, count * sizeof(uint32_t), m_localData->getData());
	}

}
