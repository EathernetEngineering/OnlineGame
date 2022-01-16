#include <Engine/pch.h>
#include <Engine/Shader.h>

#include <Engine/Platform/OpenGLShader.h>

#include <Engine/RendererAPI.h>

namespace cee
{
	namespace engine
	{
		std::shared_ptr<Shader> Shader::Create(const std::string& vertexFilename, const std::string& fragmentFilename)
		{
			switch (RendererAPI::GetAPI())
			{
				case RendererAPI::API::NONE: CEE_CORE_ASSERT(false, "RendererAPI::API::NONE is not vaild.");
				case RendererAPI::API::OPENGL: return std::make_shared<OpenGLShader>(vertexFilename, fragmentFilename);
			}
			CEE_CORE_ASSERT(false, "RendererAPI::API::NONE is not vaild.\n");
			return nullptr;
		}
	}
}
