#include "CometPCH.h"
#include "OpenGLVertexBuffer.h"

#include "glad/glad.h"

namespace Comet
{

static GLenum getOpenGLUsage(VertexBuffer::Usage usage)
{
	switch (usage)
	{
	case VertexBuffer::Usage::STATIC:    return GL_STATIC_DRAW; break;
	case VertexBuffer::Usage::DYNAMIC:   return GL_DYNAMIC_DRAW; break;
	}

	CMT_COMET_ASSERT_MESSAGE(false, "Unknown vertex buffer usage");
	return 0;
}


OpenGLVertexBuffer::OpenGLVertexBuffer(const void* const data, const uint32_t size, const Usage usage)
	: m_size(size), m_usage(usage)
{
	m_localData = Buffer::create(data, size);

	glCreateBuffers(1, &m_rendererID);
	glNamedBufferData(m_rendererID, m_size, m_localData->getData(), getOpenGLUsage(m_usage));
}

OpenGLVertexBuffer::OpenGLVertexBuffer(const uint32_t size, const Usage usage)
	: m_localData(nullptr), m_size(size), m_usage(usage)
{
	glCreateBuffers(1, &m_rendererID);
	glNamedBufferData(m_rendererID, m_size, nullptr, getOpenGLUsage(m_usage));
}

OpenGLVertexBuffer::~OpenGLVertexBuffer()
{
	glDeleteBuffers(1, &m_rendererID);
}

void OpenGLVertexBuffer::bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
}

void OpenGLVertexBuffer::setData(const void* const data, const uint32_t size, const uint32_t offset)
{
	m_localData = Buffer::create(data, size);

	glNamedBufferSubData(m_rendererID, offset, size, m_localData->getData());
}

}