#include "ClientLayer.h"
#include <common/CeeNet.hpp>

enum class MessageTypes
{
	ServerAccept = 0,
	ServerDeny = 1,
	ServerPing = 2,
	MessageAll = 3,
	ServerMessage = 4
};

void Client::PingServer()
{
	Message msg;
	msg.header.id = MessageTypes::ServerPing;
	std::chrono::system_clock::time_point startTime = std::chrono::system_clock::now();
	msg << startTime;
	Send(msg);
}

void Client::MessageAll()
{
	Message msg;
	msg.header.id = MessageTypes::MessageAll;
	Send(msg);
}

ClientLayer::ClientLayer(const std::string& debugName, uint16_t port)
	: Layer(debugName), m_Port(port)
{
	cee::net::SetErrorCallback([](cee::net::ErrorSeverity severity, const std::string& message)
	{
		using namespace cee::net;
		switch (severity)
		{
			case ErrorSeverity::TRACE:
			{
				CEE_TRACE("cee::net Error: {0}", message);
			}
			break;
			
			case ErrorSeverity::INFO:
			{
				CEE_INFO("cee::net Error: {0}", message);
			}
			break;
			
			case ErrorSeverity::WARN:
			{
				CEE_WARN("cee::net Error: {0}", message);
			}
			break;
			
			case ErrorSeverity::ERROR:
			{
				CEE_ERROR("cee::net Error: {0}", message);
			}
			break;
			
			case ErrorSeverity::CRITICAL:
			{
				CEE_CRITICAL("cee::net Error: {0}", message);
			}
			break;
			
			default:
			{
				CEE_WARN("cee::net Error (Unknown Severity): {0}", message);
			}
			break;
		}
	});
}

void ClientLayer::OnAttach()
{
	m_Client.reset(new Client);
	m_Client->Connect("127.0.0.1", m_Port);
}

void ClientLayer::OnDetach()
{
	m_Client.reset();
}

void ClientLayer::OnEvent(cee::engine::Event& e)
{
}

void ClientLayer::OnUpdate()
{
	if (m_Client->IsConnected())
		{
			if (!m_Client->Incoming().empty())
			{
				auto msg = m_Client->Incoming().pop_front().msg;
				switch(msg.header.id)
				{
					case MessageTypes::ServerAccept:
					{
						CEE_INFO("Server Accept Confrimed");
					}
					break;
					
					case MessageTypes::ServerPing:
					{
						std::chrono::system_clock::time_point endTime = std::chrono::system_clock::now();
						std::chrono::system_clock::time_point startTime;
						msg >> startTime;
						CEE_INFO("Ping: {0}ms", std::chrono::duration<float>(endTime - startTime).count());
					}
					break;
					
					case MessageTypes::ServerMessage:
					{
						uint32_t clientId;
						msg >> clientId;
						CEE_TRACE("Hello from [{0}]", clientId);
					}
					break;
					
					default:
					break;
				}
			}
			char keyDown = 0x00;
			read(fileno(stdin), &keyDown, 1);
			if (keyDown == 'p')
			{
				m_Client->PingServer();
			}
			if (keyDown == 'a')
			{
				m_Client->MessageAll();
			}
			if (keyDown == 'q')
			{
				;
			}
		}
}

void ClientLayer::OnLateUpdate()
{
}

void ClientLayer::OnFixedUpdate()
{
}

void ClientLayer::OnRender()
{
}

void ClientLayer::OnImGuiRender()
{
}
