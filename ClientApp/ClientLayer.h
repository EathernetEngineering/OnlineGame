#pragma once
#include <Engine/Engine.h>
#include <common/CeeNet.hpp>

enum class MessageTypes
{
	ServerAccept = 0,
	ServerDeny = 1,
	ServerPing = 2,
	MessageAll = 3,
	ServerMessage = 4
};

class Client : public cee::net::ClientInterface<MessageTypes>
{
public:
	Client() = default;
	
public:
	void PingServer();
	void MessageAll();
};

class ClientLayer : public cee::engine::Layer
{
public:
	ClientLayer(const std::string& debugName)
		: Layer(debugName)
	{
	}
	
	virtual void OnAttach() override = 0;
	virtual void OnDetach() override = 0;
	
	virtual void OnEvent(cee::engine::Event& e) override = 0;
	virtual void OnUpdate() override = 0;
	
	virtual void OnLateUpdate() = 0;
	virtual void OnFixedUpdate() = 0;
	virtual void OnRender() = 0;
	
	virtual void OnImGuiRender() override = 0;
};
