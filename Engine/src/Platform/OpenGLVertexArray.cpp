#include <Engine/pch.h>
#include <Engine/Platform/OpenGLVertexArray.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace cee
{
	namespace engine
	{
		static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
		{
			switch (type)
			{
				case ShaderDataType::Float:   return GL_FLOAT;
				case ShaderDataType::Float2:  return GL_FLOAT;
				case ShaderDataType::Float3:  return GL_FLOAT;
				case ShaderDataType::Float4:  return GL_FLOAT;
				case ShaderDataType::Mat3:    return GL_FLOAT;
				case ShaderDataType::Mat4:    return GL_FLOAT;
				case ShaderDataType::Int:     return GL_INT;
				case ShaderDataType::Int2:    return GL_INT;
				case ShaderDataType::Int3:    return GL_INT;
				case ShaderDataType::Int4:    return GL_INT;
				case ShaderDataType::Bool:    return GL_BOOL;
				default:
					break;
			}
			CEE_CORE_ERROR("Unknown ShaderDataType.");
			return 0;
		}
		
		OpenGLVertexArray::OpenGLVertexArray()
		{
			glCreateVertexArrays(1, &m_RendererId);
		}
		
		OpenGLVertexArray::~OpenGLVertexArray()
		{
			glDeleteVertexArrays(1, &m_RendererId);
		}
		
		void OpenGLVertexArray::Bind() const
		{
			glBindVertexArray(m_RendererId);
		}
		
		void OpenGLVertexArray::Unbind() const
		{
			glBindVertexArray(0);
		}
		
		void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
		{
			CEE_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout.");
			
			this->Bind();
			vertexBuffer->Bind();
			
			const auto& layout = vertexBuffer->GetLayout();
			for (const auto& element : layout)
			{
				switch (element.Type)
				{
					case ShaderDataType::Float:
					case ShaderDataType::Float2:
					case ShaderDataType::Float3:
					case ShaderDataType::Float4:
					{
						glEnableVertexAttribArray(m_VertexBufferIndex);
						glVertexAttribPointer(
							m_VertexBufferIndex,
							element.GetComponantCount(),
							ShaderDataTypeToOpenGLBaseType(element.Type),
							element.Normalized ? GL_TRUE : GL_FALSE,
							layout.GetStride(),
							(const void*)element.Offset);
						m_VertexBufferIndex++;
					}
					break;
					
					case ShaderDataType::Int:
					case ShaderDataType::Int2:
					case ShaderDataType::Int3:
					case ShaderDataType::Int4:
					case ShaderDataType::Bool:
					{
						glEnableVertexAttribArray(m_VertexBufferIndex);
						glVertexAttribIPointer(
							m_VertexBufferIndex,
							element.GetComponantCount(),
							ShaderDataTypeToOpenGLBaseType(element.Type),
							layout.GetStride(),
							(const void*)element.Offset);
						m_VertexBufferIndex++;
					}
					break;
					
					case ShaderDataType::Mat3:
					case ShaderDataType::Mat4:
					{
						uint8_t count = element.GetComponantCount();
						for (uint8_t i = 0; i < count; i++)
						{
							glEnableVertexAttribArray(m_VertexBufferIndex);
							glVertexAttribPointer(
								m_VertexBufferIndex,
								count,
								ShaderDataTypeToOpenGLBaseType(element.Type),
								element.Normalized ? GL_TRUE : GL_FALSE,
								layout.GetStride(),
								(const void*)(element.Offset + sizeof(float) * count * i));
							glVertexAttribDivisor(m_VertexBufferIndex, 1);
							m_VertexBufferIndex++;
						}
					}
					break;
					
					default: CEE_CORE_ASSERT(false, "Unknown ShaderDataType.");
				}
			}
			m_VertexBuffers.push_back(vertexBuffer);
			this->Unbind();
		}
		
		void OpenGLVertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
		{
			this->Bind();
			indexBuffer->Bind();
			m_IndexBuffer = indexBuffer;
			
			this->Unbind();
		}
	}
}
