#include <Engine/pch.h>
#include <Engine/Renderer.h>

#include <Engine/RenderCommand.h>
#include <Engine/Renderer2D.h>

namespace cee
{
	namespace engine
	{
		std::unique_ptr<Renderer::SceneData> Renderer::s_SceneData = std::make_unique<Renderer::SceneData>();
		
		void Renderer::Init()
		{
			RenderCommand::Init();
			Renderer2D::Init();
		}
		
		void Renderer::Shutdown()
		{
			Renderer2D::Shutdown();
		}
		
		bool Renderer::OnWindowResize(WindowResizeEvent& e)
		{
			RenderCommand::SetViewport(0, 0, e.GetWidth(), e.GetHeight());
			return true;
		}
		
		void Renderer::BeginScene(OrthographicCamera& camera)
		{
			s_SceneData->Viewprojectionmatrix = camera.GetViewProjectionMatrix();
		}
		
		void Renderer::EndScene()
		{
			
		}
		
		void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform)
		{
			shader->Bind();
			vertexArray->Bind();
			RenderCommand::DrawIndexed(vertexArray);
		}
	}
}
