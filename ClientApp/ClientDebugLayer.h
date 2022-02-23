#pragma once
#include "ClientLayer.h"

class ClientDebugLayer : public ClientLayer
{
public:
	ClientDebugLayer();
	virtual ~ClientDebugLayer();
	
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	
	virtual void OnEvent(cee::engine::Event& e) override;
	virtual void OnUpdate() override;
	
	virtual void OnLateUpdate() override;
	virtual void OnFixedUpdate() override;
	virtual void OnRender() override;
	
	virtual void OnImGuiRender() override;
	
private:
	std::array<float, 1000> m_FrameTimes;
};
