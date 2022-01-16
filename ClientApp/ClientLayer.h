#pragma once
#include <Engine/Engine.h>
#include <common/CeeNet.hpp>

enum class MessageTypes;

class Client : public cee::net::ClientInterface<MessageTypes>
{
public:
	Client() = default;
	virtual ~Client() = default;
	
public:
	void PingServer();
	void MessageAll();
};

class ClientLayer : public cee::engine::Layer
{
public:
	ClientLayer(const std::string& debugName, uint16_t port = 60000);
	
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
