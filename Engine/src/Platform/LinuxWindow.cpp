#include <Engine/pch.h>
#include <Engine/Platform/LinuxWindow.h>

#include <cstdio>

namespace cee
{
	namespace engine
	{
		uint32_t LinuxWindow::s_GlfwWindowCount = 0;
		
		void LinuxWindow::GLFWErrorCallback(int error, const char* description)
		{
			CEE_CORE_ERROR("GLFW Error: {0}, {1}", error, description);
		}
		
		LinuxWindow::LinuxWindow(uint32_t width, uint32_t height, const std::string& title)
			: Window(width, height, title), m_Window(nullptr)
		{
		}
		
		LinuxWindow::~LinuxWindow()
		{
			if (m_Window) DestroyWindow();
		}
		
		void LinuxWindow::OnUpdate()
		{
			glfwPollEvents();
			if (m_Context) m_Context->SwapBuffers();
		}
		
		bool LinuxWindow::Init()
		{
			if (s_GlfwWindowCount == 0) {
#if defined(CEE_DEBUG)
				glfwSetErrorCallback(GLFWErrorCallback);
#endif
				if (!glfwInit())
				{
					CEE_CORE_ASSERT(false, "Failed to initalize GLFW");
				}
				
				//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
				//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
			}
			
#if defined(CEE_DEBUG)
			glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif
			
			m_Window = glfwCreateWindow(m_Data.Width, m_Data.Height, m_Data.Title.c_str(), NULL, NULL);
			if (!m_Window)
			{
				CEE_CORE_ERROR("Failed to create window");
				return false;
			}
			m_Context = Context::Create(m_Window);
			m_Context->Init();
			s_GlfwWindowCount++;
			
			glfwSetWindowUserPointer(m_Window, &m_Data);
			SetVSync(1);
			
			glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer((window));
				
				data.Width = width;
				data.Height = height;
				
				WindowResizeEvent event(width, height);
				if (data.EventCallbackFn) data.EventCallbackFn(event);
			});
			
			glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				WindowCloseEvent event;
				if (data.EventCallbackFn) data.EventCallbackFn(event);
			});
			
 			glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				
				switch (action)
				{
					case GLFW_PRESS:
					{
						KeyPressedEvent event (key, 0);
						if (data.EventCallbackFn) data.EventCallbackFn(event);
					}
					break;
					
					case GLFW_RELEASE:
					{
						KeyReleasedEvent event (key);
						if (data.EventCallbackFn) data.EventCallbackFn(event);
					}
					break;
					
					case GLFW_REPEAT:
					{
						KeyPressedEvent event (key, 1);
						if (data.EventCallbackFn) data.EventCallbackFn(event);
					}
					break;
				}
			});
			
			glfwSetCharCallback(m_Window, [](GLFWwindow* window, uint32_t keycode)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				KeyTypedEvent event(keycode);
				if (data.EventCallbackFn) data.EventCallbackFn(event);
			});
			
			glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				
				switch (action)
				{
					case GLFW_PRESS:
					{
						MouseButtonPressedEvent event(button);
						if (data.EventCallbackFn) data.EventCallbackFn(event);
					}
					break;
					
					case GLFW_RELEASE:
					{
						MouseButtonReleasedEvent event(button);
						if (data.EventCallbackFn) data.EventCallbackFn(event);
					}
					break;
				}
			});
			
			glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				MouseScrolledEvent event((float)xOffset, (float)yOffset);
				if (data.EventCallbackFn) data.EventCallbackFn(event);
			});
			
			glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				MouseMovedEvent event((float)xPos, (float)yPos);
				if (data.EventCallbackFn) data.EventCallbackFn(event);
			});
			
			return true;
		}
		
		void LinuxWindow::DestroyWindow()
		{
			glfwDestroyWindow(m_Window);
			s_GlfwWindowCount--;
			
			if (s_GlfwWindowCount<= 0)
			{
				glfwTerminate();
			}
		}
		
		void LinuxWindow::SetVSync(int VSync)
		{
			m_Data.VSync = VSync & 1;
			
			glfwSwapInterval(m_Data.VSync);
		}
	}
}
