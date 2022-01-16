#pragma once
#include <Engine/RendererAPI.h>

#include <Engine/VertexArray.h>

namespace cee
{
	namespace engine
	{
		class OpenGLRendererAPI : public RendererAPI
		{
		public:
			virtual ~OpenGLRendererAPI() = default;
			
			virtual void Init() override;
			virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
			virtual void SetClearColor(glm::vec4 color) override;
			virtual void Clear() override;
			virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount = 0) override;
		};
	}
}
