#pragma once
#include <Engine/LayerStack.h>
#include <Engine/Event.h>
#include <Engine/Window.h>

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
			
		private:
			std::unique_ptr<Window> m_Window;
			LayerStack m_LayerStack;
			
		private:
			bool m_Running = false;
		};
	}
}
