#include <Engine/pch.h>
#include <Engine/UniformBuffer.h>

#include <Engine/RendererAPI.h>
#include <Engine/Platform/OpenGLUniformBuffer.h>

namespace cee
{
	namespace engine
	{
		std::shared_ptr<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding)
		{
			switch (RendererAPI::GetAPI())
			{
				case RendererAPI::API::NONE: CEE_CORE_ASSERT(false, "RendererAPI::API::NONE is not vaild.");
				case RendererAPI::API::OPENGL: return std::make_shared<OpenGLUniformBuffer>(size, binding);
			}
			CEE_CORE_ASSERT(false, "RendererAPI::API::NONE is not vaild.\n");
			return nullptr;
		}
	}
}
