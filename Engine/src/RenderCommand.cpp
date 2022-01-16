#include <Engine/pch.h>
#include <Engine/RenderCommand.h>

namespace cee
{
	namespace engine
	{
		std::unique_ptr<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();
	}
}
