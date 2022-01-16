#pragma once
#include <string>
#include <functional>
#include <memory>
#include <Engine/Event.h>

namespace cee
{
	namespace engine
	{
		class Window
		{
		public:
			Window(uint32_t width = 1280u, uint32_t height = 720u, const std::string& title = "Application");
			virtual ~Window() = default;
			
			uint32_t GetWidth() const { return m_Data.Width; }
			uint32_t GetHeight() const { return m_Data.Height; }
			
			void SetEventCallbackFn(std::function<void(Event&)> callback) { m_Data.EventCallbackFn = callback; }
			
			int IsVSync() const { return m_Data.VSync; }
			virtual void SetVSync(int enabled) = 0;
			
			virtual void OnUpdate() = 0;
			
			virtual bool Init() = 0;
			virtual void DestroyWindow() = 0;
			
			virtual void* GetNativeWindowPtr() = 0;
			
		public:
			struct WindowData
			{
				uint32_t Width, Height;
				std::string Title;
				int VSync = 1;
				
				std::function<void(Event&)> EventCallbackFn;
			};
			
		protected:
			WindowData m_Data;
			
		public:
			static std::unique_ptr<Window> CreateWindow(uint32_t width = 1280u, uint32_t height = 720u, const std::string& title = "Application");
		};
	}
}
