#include <Engine/pch.h>
#include <Engine/Platform/OpenGLUniformBuffer.h>

#include <glad/glad.h>

namespace cee
{
	namespace engine
	{
		OpenGLUniformBuffer::OpenGLUniformBuffer(uint32_t size, uint32_t binding)
		{
			glCreateBuffers(1, &m_RendererId);
			glNamedBufferData(m_RendererId, size, nullptr, GL_DYNAMIC_DRAW);
			glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_RendererId);
		}
		
		OpenGLUniformBuffer::~OpenGLUniformBuffer()
		{
			glDeleteBuffers(1, &m_RendererId);
		}
		
		void OpenGLUniformBuffer::SetData(const void* data, uint32_t size, uint32_t offset)
		{
			glNamedBufferSubData(m_RendererId, offset, size, data);
		}
	}
}
