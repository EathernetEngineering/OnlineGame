#pragma once
#include <glm/glm.hpp>

#include <Engine/Camera.h>

namespace cee
{
	namespace engine
	{
		class Renderer2D
		{
		public:
			static void Init();
			static void Shutdown();
			
			static void BeginScene(const OrthographicCamera& camera);
			static void EndScene();
			
			static void Flush();
			
			static void DrawQuad(const glm::vec3 position, const glm::vec2 scale, const glm::vec4 color);
			
		public:
			struct Statistics
			{
				uint32_t DrawCalls;
				uint32_t QuadCount;
				
				uint32_t GetTotalVertexCount() const { return QuadCount * 4; }
				uint32_t GetTotalIndexCount()  const { return QuadCount * 6; }
			};
			
			static void ResetStats();
			static Statistics GetStats();
			
		private:
			static void StartBatch();
			static void NextBatch();
		};
	}
}
