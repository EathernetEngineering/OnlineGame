#include <Engine/pch.h>
#include <Engine/Context.h>

#include <Engine/RendererAPI.h>
#include <Engine/Platform/OpenGLContext.h>

#include <cstdio>

namespace cee
{
	namespace engine 
	{
		std::unique_ptr<Context> Context::Create(void* window)
		{
			switch (RendererAPI::GetAPI())
			{
				case RendererAPI::API::NONE: CEE_CORE_ASSERT(false, "RendererAPI::API::NONE is not vaild.");
				case RendererAPI::API::OPENGL: return std::make_unique<OpenGLContext>(static_cast<struct GLFWwindow*>(window));
			}
			CEE_CORE_ASSERT(false, "RendererAPI::API::NONE is not vaild.\n");
			return nullptr;
		}
	}
}
