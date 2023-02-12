#include "CometPCH.h"
#include "OpenGLIndexBuffer.h"

#include "Comet/Core/Buffer.h"

#include "glad/glad.h"

namespace Comet
{

static GLenum getOpenGLUsage(IndexBuffer::Usage usage)
{
	switch (usage)
	{
	case IndexBuffer::Usage::STATIC:    return GL_STATIC_DRAW; break;
	case IndexBuffer::Usage::DYNAMIC:   return GL_DYNAMIC_DRAW; break;
	default:
		CMT_COMET_ASSERT_MESSAGE(false, "Unknown index buffer usage");
		return 0;
	}
}

OpenGLIndexBuffer::OpenGLIndexBuffer(const void* const data, const uint32_t count, const Usage usage)
	: m_count(count), m_usage(usage)
{
	m_localData = Buffer::create(data, count * sizeof(uint32_t));

	glCreateBuffers(1, &m_rendererID);
	glNamedBufferData(m_rendererID, count * sizeof(uint32_t), m_localData->getData(), getOpenGLUsage(usage));
}

OpenGLIndexBuffer::OpenGLIndexBuffer(const uint32_t count, const Usage usage)
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

void OpenGLIndexBuffer::setData(const void* const data, const uint32_t count, const uint32_t offset)
{
	m_localData = Buffer::create(data, count * sizeof(uint32_t));

	glNamedBufferSubData(m_rendererID, offset, count * sizeof(uint32_t), m_localData->getData());
}

}
