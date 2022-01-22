#pragma once
#include <Engine/LayerStack.h>
#include <Engine/Event.h>
#include <Engine/Window.h>

#include <Engine/OrthographicCameraController.h>
#include <Engine/ImGuiLayer.h>

namespace cee
{
	namespace engine
	{
		class Application
		{
		public:
			Application();
			virtual ~Application();
			
		public:
			void Run();
			
			void PushLayer(Layer* layer);
			void PopLayer(Layer* layer);
			void PushOverlay(Layer* overlay);
			void PopOverlay(Layer* overlay);
			
		public:
			virtual void OnAwake() = 0;
			virtual void OnStart() = 0;
			
			void OnEvent(Event& e);
			bool OnWindowClose(WindowCloseEvent& e);
			bool OnWindowResize(WindowResizeEvent& e);
			
		public:
			Window& GetWindow() { return *m_Window; }
			float GetPreviousFrameTime() { return m_PrevFrameTime; }
			float GetAverageFrameTime() { return m_AverageFrameTime; }
			
			static size_t GetHeapMemoryAllocated() { return s_HeapMemoryAllocated; }
			
			static Application& Get() { return *s_Instance; }
			
		private:
			std::unique_ptr<Window> m_Window;
			LayerStack m_LayerStack;
			
			ImGuiLayer* m_ImGuiLayer;
			
		private:
			float m_PrevFrameTime = 0.0f;
			float m_AverageFrameTime = 0.0f;
			size_t m_FrameIndex = 0;
			
		private:
			bool m_Running = false;
			static Application* s_Instance;
			
		private:
			friend void* ::operator new(std::size_t sz);
			friend void ::operator delete(void* p) noexcept;
			
			static size_t s_HeapMemoryAllocated;
		};
	}
}
