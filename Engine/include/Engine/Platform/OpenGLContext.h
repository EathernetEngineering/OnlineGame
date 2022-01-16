#pragma once
#include <Engine/Context.h>

struct GLFWwindow;

namespace cee
{
	namespace engine
	{
		class OpenGLContext : public Context
		{
		public:
			OpenGLContext(GLFWwindow* windowHandle);
			
			virtual void Init() override;
			virtual void SwapBuffers() override;
			
		private:
			GLFWwindow* m_WindowHandle;
		};
	}
}
