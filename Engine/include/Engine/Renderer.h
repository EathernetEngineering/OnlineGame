#pragma once

#include <Engine/RendererAPI.h>
#include <Engine/Camera.h>
#include <Engine/Shader.h>
#include <Engine/VertexArray.h>
#include <Engine/Event.h>

#include <glm/glm.hpp>

namespace cee
{
	namespace engine
	{
		class Renderer
		{
		public:
			virtual ~Renderer() = default;
			
			static void Init();
			static void Shutdown();
			
			static bool OnWindowResize(WindowResizeEvent& e);
			
			static void BeginScene(OrthographicCamera& camera);
			static void EndScene();
			
			static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));
			
			static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
		private:
			struct SceneData
			{
				glm::mat4 Viewprojectionmatrix;
			};
			
			static std::unique_ptr<SceneData> s_SceneData;
		};
	}
}
