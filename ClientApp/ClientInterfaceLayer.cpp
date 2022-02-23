#include "ClientInterfaceLayer.h"

#include <imgui.h>

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
	
	FramebufferSpecification fbSpec;
	fbSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
	fbSpec.Width = 1280;
	fbSpec.Height = 720;
	m_Framebuffer = Framebuffer::Create(fbSpec);
	
	m_CameraController = std::make_unique<OrthographicCameraController>(m_Framebuffer, true);
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
	static bool dockspaceOpen = true;
	static bool opt_fullscreen_persistant = true;
	bool opt_fullscreen = opt_fullscreen_persistant;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
	
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}
	
	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;
	
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("Dockspace", &dockspaceOpen, window_flags);
	ImGui::PopStyleVar();
	
	if (opt_fullscreen)
		ImGui::PopStyleVar(2);
	
	ImGuiIO& io = ImGui::GetIO();
	ImGuiStyle& style = ImGui::GetStyle();
	float minWinSizeX = style.WindowMinSize.x;
	style.WindowMinSize.x = 370.0f;
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}
	
	style.WindowMinSize.x = minWinSizeX;
	
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit")) cee::engine::Application::Get().Close();
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	
	ImGui::Begin("Properties");
	ImGui::ColorEdit3("Clear Color", m_ClearColor);
	ImGui::ColorEdit3("Quad Color", m_QuadColor);
	ImGui::End();
	
	ImGui::Begin("Scene");
	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
	
	uint64_t textureId = m_Framebuffer->GetColorAttachmentRendererId();
	ImGui::Image(reinterpret_cast<void*>(textureId), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
	
	ImGui::End();
	ImGui::End();
}

bool ClientInterfaceLayer::OnWindowResize(cee::engine::WindowResizeEvent& e)
{
	return true;
}

