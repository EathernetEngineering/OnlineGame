#include <Engine/pch.h>
#include <Engine/RendererAPI.h>
#include <Engine/Platform/OpenGLRendererAPI.h>

#include <cstdio>

namespace cee
{
	namespace engine
	{
		RendererAPI::API RendererAPI::s_API = RendererAPI::API::OPENGL;
		
		std::unique_ptr<RendererAPI> RendererAPI::Create()
		{
			switch (RendererAPI::GetAPI())
			{
				case RendererAPI::API::NONE: CEE_CORE_ASSERT(false, "RendererAPI::API::NONE is not vaild.");
				case RendererAPI::API::OPENGL: return std::make_unique<OpenGLRendererAPI>();
			}
			return nullptr;
		}
	}
}
