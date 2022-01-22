#include <Engine/pch.h>
#include <Engine/Framebuffer.h>

#include <Engine/RendererAPI.h>
#include <Engine/Platform/OpenGLFramebuffer.h>


namespace cee
{
	namespace engine
	{
		std::shared_ptr<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
		{
			switch (RendererAPI::GetAPI())
			{
				case RendererAPI::API::NONE: CEE_CORE_ASSERT(false, "RendererAPI::API::NONE is not vaild.");
				case RendererAPI::API::OPENGL: return std::make_shared<OpenGLFramebuffer>(spec);
			}
			CEE_CORE_ASSERT(false, "RendererAPI::API::NONE is not vaild.\n");
			return nullptr;
		}
	}
}
