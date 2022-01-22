#pragma once
#include "ClientLayer.h"

class ClientNetLayer : public ClientLayer
{
public:
	ClientNetLayer(const std::string& debugName, uint16_t port = 60000);
	
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	
	virtual void OnEvent(cee::engine::Event& e) override;
	virtual void OnUpdate() override;
	
	virtual void OnLateUpdate() override;
	virtual void OnFixedUpdate() override;
	virtual void OnRender() override;
	
	virtual void OnImGuiRender() override;
	
private:
	uint16_t m_Port;
	std::unique_ptr<Client> m_Client;
};
