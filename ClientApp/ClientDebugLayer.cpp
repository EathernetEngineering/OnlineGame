#include "ClientDebugLayer.h"

#include <string>
#include <imgui.h>

ClientDebugLayer::ClientDebugLayer()
	: ClientLayer("ClientDebugLayer")
{
}

ClientDebugLayer::~ClientDebugLayer()
{
}

void ClientDebugLayer::OnAttach()
{
	m_FrameTimes.fill(0.0f);
}

void ClientDebugLayer::OnDetach()
{
}

void ClientDebugLayer::OnEvent(cee::engine::Event& e)
{
}

void ClientDebugLayer::OnUpdate()
{
}

void ClientDebugLayer::OnLateUpdate()
{
}

void ClientDebugLayer::OnFixedUpdate()
{
}

void ClientDebugLayer::OnRender()
{
}

void ClientDebugLayer::OnImGuiRender()
{
	static uint32_t s_FrameTimeIndex = 0;
	if (s_FrameTimeIndex > 999) s_FrameTimeIndex = 0;
	
	m_FrameTimes[s_FrameTimeIndex] = cee::engine::Application::Get().GetPreviousFrameTime();
	s_FrameTimeIndex++;
	
	ImGui::Begin("Debug Stats");
	ImGui::PlotLines("Frame Times", m_FrameTimes.data(), m_FrameTimes.size());
	float avgFrameTime = cee::engine::Application::Get().GetAverageFrameTime();
	ImGui::Text("Average Frame Time: %.3fms (%iFPS)", avgFrameTime, (int)(1000.0f/avgFrameTime));
	ImGui::End();
}
