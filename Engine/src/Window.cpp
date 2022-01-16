#include <Engine/pch.h>
#include <Engine/Window.h>
#include <Engine/Platform/LinuxWindow.h>

namespace cee
{
	namespace engine
	{
		Window::Window(uint32_t width, uint32_t height, const std::string& title)
		{
			m_Data.Width = width;
			m_Data.Height = height;
			m_Data.Title = title;
		}
		
		std::unique_ptr<Window> Window::CreateWindow(uint32_t width, uint32_t height, const std::string& title)
		{
			return std::make_unique<LinuxWindow>(width, height, title);
		}
	}
}
