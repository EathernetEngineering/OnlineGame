#include <Engine/pch.h>
#include <Engine/Buffer.h>

#include <Engine/RendererAPI.h>

#include <Engine/Platform/OpenGLBuffer.h>

namespace cee
{
	namespace engine
	{
		std::shared_ptr<VertexBuffer> VertexBuffer::Create(uint32_t size)
		{
			switch (RendererAPI::GetAPI())
			{
				case RendererAPI::API::NONE: CEE_CORE_ASSERT(false, "RendererAPI::API::NONE is not vaild.");
				case RendererAPI::API::OPENGL: return std::make_unique<OpenGLVertexBuffer>(size);
			}
			return nullptr;
		}
		
		std::shared_ptr<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
		{
			switch (RendererAPI::GetAPI())
			{
				case RendererAPI::API::NONE: CEE_CORE_ASSERT(false, "RendererAPI::API::NONE is not vaild.");
				case RendererAPI::API::OPENGL: return std::make_unique<OpenGLVertexBuffer>(vertices, size);
			}
			return nullptr;
		}
		
		std::shared_ptr<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
		{
			switch (RendererAPI::GetAPI())
			{
				case RendererAPI::API::NONE: CEE_CORE_ASSERT(false, "RendererAPI::API::NONE is not vaild.");
				case RendererAPI::API::OPENGL: return std::make_unique<OpenGLIndexBuffer>(indices, count);
			}
			return nullptr;
		}
	}
}
