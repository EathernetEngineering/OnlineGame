#include <Engine/pch.h>
#include <Engine/Platform/OpenGLContext.h>

#include <cstdio>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace cee
{
	namespace engine
	{
		OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
			: m_WindowHandle(windowHandle)
		{
			if (!m_WindowHandle) CEE_CORE_ERROR("`windowHandle != NULL` Failed");
		}
			
		void OpenGLContext::Init()
		{
			glfwMakeContextCurrent(m_WindowHandle);
			int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
			if (!status) CEE_CORE_ASSERT(false, "Failed to initalize Glad");
			
			CEE_CORE_INFO("OpenGL info:");
			CEE_CORE_INFO("  Vendor:    {0}", glGetString(GL_VENDOR));
			CEE_CORE_INFO("  Renderer:  {0}", glGetString(GL_RENDERER));
			CEE_CORE_INFO("  Version:   {0}", glGetString(GL_VERSION));
		}
		
		void OpenGLContext::SwapBuffers()
		{
			glfwSwapBuffers(m_WindowHandle);
		}
	}
}
