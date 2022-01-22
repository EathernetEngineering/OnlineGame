#include "ClientInterfaceLayer.h"

#include <imgui.h>

#include <Engine/RenderCommand.h>
#include <Engine/Renderer2D.h>

ClientInterfaceLayer::ClientInterfaceLayer(const std::string& debugName)
	: ClientLayer(debugName)
{
	for (uint32_t i = 0; i < IM_ARRAYSIZE(m_ClearColor); i++)
	{
		m_ClearColor[i] = 0.0f;
		m_QuadColor[i] = 0.0f;
	}
}

void ClientInterfaceLayer::OnAttach()
{
	using namespace cee::engine;
	m_CameraController = std::make_unique<OrthographicCameraController>((Application::Get().GetWindow().GetWidth()/Application::Get().GetWindow().GetHeight()), true);
	
	FramebufferSpecification fbSpec;
	fbSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
	fbSpec.Width = 1280;
	fbSpec.Height = 720;
	m_Framebuffer = Framebuffer::Create(fbSpec);
	
	m_FrameTimes.fill(0.0f);
}

void ClientInterfaceLayer::OnDetach()
{
}

void ClientInterfaceLayer::OnEvent(cee::engine::Event& e)
{
	m_CameraController->OnEvent(e);
	cee::engine::EventDispatcher d(e);
	
	d.Dispatch<cee::engine::WindowResizeEvent>(CEE_BIND_EVENT_FN(OnWindowResize));
}

void ClientInterfaceLayer::OnUpdate()
{
	if (cee::engine::FramebufferSpecification spec = m_Framebuffer->GetSpecification();
		m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&
		(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
	{
		m_CameraController->OnResize(m_ViewportSize.x, m_ViewportSize.y);
		m_Framebuffer->Resize(m_ViewportSize.x, m_ViewportSize.y);
	}
	
	using namespace cee::engine;
	Renderer2D::ResetStats();
	m_Framebuffer->Bind();
	RenderCommand::SetClearColor({ m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], 1.0f });
	RenderCommand::Clear();
	m_CameraController->OnUpdate();
	
	Renderer2D::BeginScene(m_CameraController->GetCamera());
	Renderer2D::DrawQuad({ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, { m_QuadColor[0], m_QuadColor[1], m_QuadColor[2], 1.0f });
	Renderer2D::EndScene();
	m_Framebuffer->Unbind();
}

void ClientInterfaceLayer::OnLateUpdate()
{
}

void ClientInterfaceLayer::OnFixedUpdate()
{
}

void ClientInterfaceLayer::OnRender()
{
}

void ClientInterfaceLayer::OnImGuiRender()
{
	static uint32_t s_FrameTimeIndex = 0;
	if (s_FrameTimeIndex > 1000) s_FrameTimeIndex = 0;
	
	m_FrameTimes[s_FrameTimeIndex] = cee::engine::Application::Get().GetPreviousFrameTime();
	s_FrameTimeIndex++;
	
	if (ImGui::Begin("Debug Stats"))
	{
		ImGui::PlotLines("Frame Times", m_FrameTimes.data(), m_FrameTimes.size());
		float avgFrameTime = cee::engine::Application::Get().GetAverageFrameTime();
		ImGui::Text("Average Frame Time: %.3fms, Frame Rate %i", avgFrameTime, (int)(1000.0f/avgFrameTime));
		ImGui::Text("Current Heap Memory Allocated: %luB", cee::engine::Application::GetHeapMemoryAllocated());
		ImGui::End();
	}
	if (ImGui::Begin("Color Control"))
	{
		ImGui::ColorEdit3("Clear Color", m_ClearColor);
		ImGui::ColorEdit3("Quad Color", m_QuadColor);
		ImGui::End();
	}
	if (ImGui::Begin("Scene"))
	{
		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
		
		uint64_t textureId = m_Framebuffer->GetColorAttachmentRendererId();
		ImGui::Image(reinterpret_cast<void*>(textureId), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		
		ImGui::End();
	}
}

bool ClientInterfaceLayer::OnWindowResize(cee::engine::WindowResizeEvent& e)
{
	return true;
}

