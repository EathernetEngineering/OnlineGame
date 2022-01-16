#include <Engine/pch.h>
#include <Engine/VertexArray.h>

#include <Engine/RendererAPI.h>

#include <Engine/Platform/OpenGLVertexArray.h>

namespace cee
{
	namespace engine
	{
		std::shared_ptr<VertexArray> VertexArray::Create()
		{
			switch (RendererAPI::GetAPI())
			{
				case RendererAPI::API::NONE: CEE_CORE_ASSERT(false, "RendererAPI::API::NONE is not vaild.");
				case RendererAPI::API::OPENGL: return std::make_unique<OpenGLVertexArray>();
			}
			return nullptr;
		}
	}
}
