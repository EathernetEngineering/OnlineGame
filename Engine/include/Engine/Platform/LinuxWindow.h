#pragma once
#include <Engine/Window.h>
#include <Engine/Context.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace cee
{
	namespace engine
	{
		class LinuxWindow : public Window
		{
		public:
			LinuxWindow(uint32_t width = 1280u, uint32_t height = 720u, const std::string& title = "Application");
			~LinuxWindow();
			
			virtual bool Init() override;
			virtual void DestroyWindow() override;
			
			virtual void OnUpdate() override;
			virtual void* GetNativeWindowPtr() override { return nullptr; }
			
			virtual void SetVSync(int VSync) override;
			
		private:
			static void GLFWErrorCallback(int error, const char* description);
			
		private:
			GLFWwindow* m_Window;
			std::unique_ptr<Context> m_Context;
		protected:
			static uint32_t s_GlfwWindowCount;
			
		};
	}
}
