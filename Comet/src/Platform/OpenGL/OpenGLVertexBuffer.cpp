#include "CometPCH.h"
#include "OpenGLVertexBuffer.h"

#include "glad/glad.h"

namespace Comet
{

	static GLenum getOpenGLUsage(VertexBufferUsage usage)
	{
		switch (usage)
		{
			case VertexBufferUsage::STATIC:    return GL_STATIC_DRAW; break;
			case VertexBufferUsage::DYNAMIC:   return GL_DYNAMIC_DRAW; break;
		}

		CMT_COMET_ASSERT(false, "Unknown vertex buffer usage");
		return 0;
	}


	OpenGLVertexBuffer::OpenGLVertexBuffer(void* data, uint32_t size, VertexBufferUsage usage)
		: m_size(size), m_usage(usage)
	{
		//TODO: IMPLEMENT OWN BUFFER CLASS TO HAVE SECURE OWNERSHIP AND MOVEMENT
		m_localData = Buffer::create(data, size);

		glCreateBuffers(1, &m_rendererID);
		glNamedBufferData(m_rendererID, m_size, m_localData->getData(), getOpenGLUsage(m_usage));
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size, VertexBufferUsage usage)
		: m_localData(nullptr), m_size(size), m_usage(usage)
	{
		//TODO: IMPLEMENT OWN BUFFER CLASS TO HAVE SECURE OWNERSHIP AND MOVEMENT

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

	void OpenGLVertexBuffer::setData(void* data, uint32_t size, uint32_t offset)
	{
		m_localData = Buffer::create(data, size);
		m_size = size;

		glNamedBufferSubData(m_rendererID, offset, size, m_localData->getData());
	}

}