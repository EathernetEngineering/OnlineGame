#include <Engine/pch.h>
#include <Engine/Platform/OpenGLBuffer.h>

#include <glad/glad.h>

namespace cee
{
	namespace engine
	{
		/////////////////////////////////////////////////////////////////////////
		// VertexBuffer /////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////
		
		OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size)
		{
			glCreateBuffers(1, &m_RendererId);
			glBindBuffer(GL_ARRAY_BUFFER, m_RendererId);
			glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
		}
		
		OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size)
		{
			glCreateBuffers(1, &m_RendererId);
			glBindBuffer(GL_ARRAY_BUFFER, m_RendererId);
			glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
		}
		
		OpenGLVertexBuffer::~OpenGLVertexBuffer()
		{
			glDeleteBuffers(1, &m_RendererId);
		}
		
		void OpenGLVertexBuffer::Bind() const
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_RendererId);
		}
		
		void OpenGLVertexBuffer::Unbind() const
		{
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
		
		void OpenGLVertexBuffer::SetData(const void* data, uint32_t size)
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_RendererId);
			glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
		}
		
		/////////////////////////////////////////////////////////////////////////
		// IndexBuffer //////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////
		
		OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
		{
			glCreateBuffers(1, &m_RendererId);
			glBindBuffer(GL_ARRAY_BUFFER, m_RendererId);
			glBufferData(GL_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
		}
		
		OpenGLIndexBuffer::~OpenGLIndexBuffer()
		{
			glDeleteBuffers(1, &m_RendererId);
		}
		
		void OpenGLIndexBuffer::Bind() const
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererId);
		}
		
		void OpenGLIndexBuffer::Unbind() const
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}
	}
}
