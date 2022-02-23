#include <Engine/pch.h>
#include <Engine/Application.h>

#include <Engine/Renderer.h>

#include <Engine/Timer.h>

namespace cee
{
	namespace engine
	{
		Application* Application::s_Instance = nullptr;
		
		Application::Application()
		{
			if (s_Instance)
			{
				CEE_CORE_ASSERT(false, "Instance of cee::engine::Application alreay exists");
			}
			s_Instance = this;
			
			Log::Init();
			
			CEE_PROFILE_BEGIN_SESSION("Applicaton Profile", "/home/chloe/dev/c++/CEENetApp/Profile.log");
			
			m_Window = Window::CreateWindow(1280, 720, "Clinet Window");
			if (!m_Window)
			{
				CEE_CORE_CRITICAL("Failed to craete window");
				m_Running = false;
			}
			
			m_Window->SetEventCallbackFn(CEE_BIND_EVENT_FN(Application::OnEvent));
			m_Window->Init();
			
			Renderer::Init();
		}
		
		Application::~Application()
		{
			Renderer::Shutdown();
			CEE_PROFILE_END_SESSION();
		}
		
		void Application::Run()
		{
			m_Running = true;
			
			Timer timer;
			
			m_ImGuiLayer = new ImGuiLayer();
			this->PushOverlay(m_ImGuiLayer);
			
			while (m_Running)
			{
				m_PrevFrameTime = timer.ElapsedMillis();
				m_AverageFrameTime = m_PrevFrameTime + (m_PrevFrameTime - m_AverageFrameTime)/(m_FrameIndex + 1);
				m_FrameIndex++;
				timer.Reset();
				
				for (auto& layer : m_LayerStack)
				{
					layer->OnUpdate();
				}
				m_ImGuiLayer->Begin();
				for (auto& layer : m_LayerStack)
				{
					layer->OnImGuiRender();
				}
				m_ImGuiLayer->End();
				m_Window->OnUpdate();
			}
		}
		
		void Application::Close()
		{
			m_Running = false;
		}
		
		void Application::OnEvent(Event& e)
		{
			EventDispatcher d(e);
			d.Dispatch<WindowCloseEvent>(CEE_BIND_EVENT_FN(Application::OnWindowClose));
			
			for (auto& layer : m_LayerStack)
			{
				layer->OnEvent(e);
			}
		}
		
		bool Application::OnWindowClose(WindowCloseEvent& e)
		{
			m_Running = false;
			return true;
		}
		
		bool Application::OnWindowResize(WindowResizeEvent& e)
		{
			return Renderer::OnWindowResize(e);
		}
		
		void Application::PushLayer(Layer* layer)
		{
			CEE_PROFILE_FUNCTION();
			
			m_LayerStack.PushLayer(layer);
			layer->OnAttach();
		}
		
		void Application::PopLayer(Layer* layer)
		{
			m_LayerStack.PopLayer(layer);
			layer->OnAttach();
		}
		
		void Application::PushOverlay(Layer* overlay)
		{
			CEE_PROFILE_FUNCTION();
			
			m_LayerStack.PushOverlay(overlay);
			overlay->OnAttach();
		}
		
		void Application::PopOverlay(Layer* overlay)
		{
			m_LayerStack.PopOverlay(overlay);
			overlay->OnAttach();
		}
	}
}
