#include <Engine/pch.h>
#include <Engine/Application.h>

#include <Engine/RenderCommand.h>

#include <Engine/Buffer.h>
#include <Engine/VertexArray.h>

#include <Engine/Shader.h>

#include <glad/glad.h>

namespace cee
{
	namespace engine
	{
		Application::Application()
		{
			Log::Init();
		}
		
		Application::~Application()
		{
			m_Window.reset();
		}
		
		void Application::Run()
		{
			m_Running = true;
			m_Window = Window::CreateWindow(1280, 720, "Clinet Window");
			if (!m_Window)
			{
				CEE_CORE_WARN("Failed to craete window");
				m_Running = false;
			}
			
			m_Window->SetEventCallbackFn(CEE_BIND_EVENT_FN(Application::OnEvent));
			m_Window->Init();
			
			static float vertices[] = {
				-0.5f, -0.5f, 0.0f,
				-0.5f,  0.5f, 0.0f,
				 0.5f, -0.5f, 0.0f,
				 0.5f,  0.5f, 0.0f
			};
			
			static uint32_t indices[] = {
				0, 1, 2,
				2, 1, 3
			};
			
			std::shared_ptr<VertexBuffer> vertexBuffer = VertexBuffer::Create(vertices, sizeof(vertices));
			BufferLayout layout({ { ShaderDataType::Float3, "pos" } });
			vertexBuffer->SetLayout(layout);
			std::shared_ptr<IndexBuffer> indexBuffer = IndexBuffer::Create(indices, 6);
			
			vertexBuffer->Bind();
			indexBuffer->Bind();
			
			std::shared_ptr<VertexArray> vertexArray = VertexArray::Create();
			
			vertexArray->AddVertexBuffer(vertexBuffer);
			vertexArray->SetIndexBuffer(indexBuffer);
			
			std::shared_ptr<Shader> shader = Shader::Create("Engine/res/shaders/SimpleVertex.glsl", "Engine/res/shaders/SimpleFragment.glsl");
			
			while (m_Running)
			{
				RenderCommand::Clear();
				RenderCommand::SetClearColor({ 0.2f, 0.0f, 0.8f, 1.0f });
				
				shader->Bind();
				vertexArray->Bind();
				RenderCommand::DrawIndexed(vertexArray, 6);
				
				for (auto& layer : m_LayerStack)
				{
					layer->OnUpdate();
				}
				m_Window->OnUpdate();
				for (auto& layer : m_LayerStack)
				{
					layer->OnLateUpdate();
				}
			}
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
		
		void Application::PushLayer(Layer* layer)
		{
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
