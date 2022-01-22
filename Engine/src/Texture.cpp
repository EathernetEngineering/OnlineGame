#include <Engine/pch.h>
#include <Engine/Texture.h>

#include <Engine/RendererAPI.h>
#include <Engine/Platform/OpenGLTexture.h>

namespace cee
{
	namespace engine
	{
		std::shared_ptr<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
		{
			switch (RendererAPI::GetAPI())
			{
				case RendererAPI::API::NONE: CEE_CORE_ASSERT(false, "RendererAPI::API::NONE is not vaild.");
				case RendererAPI::API::OPENGL: return std::make_shared<OpenGLTexture2D>(width, height);
			}
			CEE_CORE_ASSERT(false, "RendererAPI::API::NONE is not vaild.\n");
			return nullptr;
		}
		
		std::shared_ptr<Texture2D> Texture2D::Create(const std::string& filepath)
		{
			switch (RendererAPI::GetAPI())
			{
				case RendererAPI::API::NONE: CEE_CORE_ASSERT(false, "RendererAPI::API::NONE is not vaild.");
				case RendererAPI::API::OPENGL: return std::make_shared<OpenGLTexture2D>(filepath);
			}
			CEE_CORE_ASSERT(false, "RendererAPI::API::NONE is not vaild.\n");
			return nullptr;
		}
	}
}
