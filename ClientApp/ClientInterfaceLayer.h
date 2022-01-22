#pragma once
#include "ClientLayer.h"

#include <Engine/OrthographicCameraController.h>
#include <Engine/Framebuffer.h>

#include <array>

class ClientInterfaceLayer : public ClientLayer
{
public:
	ClientInterfaceLayer(const std::string& debugName);
	
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	
	virtual void OnEvent(cee::engine::Event& e) override;
	virtual void OnUpdate() override;
	
	virtual void OnLateUpdate() override;
	virtual void OnFixedUpdate() override;
	virtual void OnRender() override;
	
	virtual void OnImGuiRender() override;
	
private:
	bool OnWindowResize(cee::engine::WindowResizeEvent& e);
	
private:
	float m_ClearColor[3];
	float m_QuadColor[3];
	
	glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
	
	std::shared_ptr<cee::engine::Framebuffer> m_Framebuffer;
	
	std::unique_ptr<cee::engine::OrthographicCameraController> m_CameraController;
	
	std::array<float, 1000> m_FrameTimes;
};
