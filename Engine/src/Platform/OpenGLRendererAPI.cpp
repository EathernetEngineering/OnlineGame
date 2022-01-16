#include <Engine/pch.h>
#include <Engine/Platform/OpenGLRendererAPI.h>

#include <glad/glad.h>

namespace cee
{
	namespace engine
	{
		void OpenGLMessageCallback(
			uint32_t source,
			uint32_t type,
			uint32_t id,
			uint32_t severity,
			int length,
			const char*
			message,
			const void* userParam)
		{
			CEE_CORE_WARN("OpenGL Error: Severity {0}, message \"{1}\"", severity, message);
		}
			
		void OpenGLRendererAPI::Init()
		{
#if defined(CEE_DEBUG)
			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			glDebugMessageCallback(OpenGLMessageCallback, nullptr);
			
			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
#endif
			
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			
			glEnable(GL_DEPTH_TEST);
		}
		
		void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			glViewport(x, y, width, height);
		}
		
		void OpenGLRendererAPI::SetClearColor(glm::vec4 color)
		{
			glClearColor(color.r, color.g, color.b, color.a);
		}
		
		void OpenGLRendererAPI::Clear()
		{
			glClear(GL_COLOR_BUFFER_BIT);
		}
		
		void OpenGLRendererAPI::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount)
		{
			uint32_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();
			glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
		}
	}
}
