#pragma once
#include <common/Common.hpp>
#include <common/TsQueue.hpp>
#include <common/Message.hpp>
#include <common/Connection.hpp>

namespace cee
{
	namespace net
	{
		template<typename T>
		class ServerInterface
		{
		public:
			using Message = message<T>;
			using OwnedMessage = owned_message<T>;
			using MessageQueue = TsQueue<Message>;
			using OwnedMessageQueue = TsQueue<OwnedMessage>;
			using ConnectionType = Connection<T>;
			using Client = std::shared_ptr<ConnectionType>;
			
		public:
			ServerInterface(uint16_t port)
			: m_AsioAcceptor(m_AsioContext, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
			{
			}
			
			virtual ~ServerInterface()
			{
				Stop();
			}
			
			bool Start()
			{
				try
				{
					WaitForClientConnection();
					m_ThreadContext = std::thread([this](){ m_AsioContext.run(); });
				}
				catch (std::exception& e)
				{
					fprintf(stderr, "[SERVER] Exception: %s\n", e.what());
					return false;
				}
				catch (boost::exception& e)
				{
					fprintf(stderr, "[SERVER] Boost Exception\n");
					return false;
				}
				printf("[SERVER] Stared!\n");
				return true;
			}
			
			void Stop()
			{
				m_AsioContext.stop();
				
				if (m_ThreadContext.joinable())
					m_ThreadContext.join();
				
				printf("[SERVER] Stopped!\n");
			}
			
			void WaitForClientConnection()
			{
				m_AsioAcceptor.async_accept(
					[this](boost::system::error_code ec, boost::asio::ip::tcp::socket socket)
					{
						if (!ec)
						{
							printf("[SERVER] New connection: %s\n", socket.remote_endpoint().address().to_string().c_str());
							Client newConnection =
								std::make_shared<ConnectionType>(ConnectionType::owner::server, m_AsioContext, std::move(socket), m_MessagesIn);
								
							if (OnClientConnect(newConnection))
							{
								m_DeqConnections.push_back(std::move(newConnection));
								m_DeqConnections.back()->ConnectToClient(++m_IdCounter);
								
								printf("[%u] Connection Approved.\n", m_DeqConnections.back()->GetId());
							}
							else
							{
								printf("[-----] Connection Denied.");
							}
						}
						else
						{
							printf("[SERVER] New Connection Error: %s\n", ec.message().c_str());
						}
						WaitForClientConnection();
					});
			}
			
			void MessageClient(Client client, const Message& msg)
			{
				if (client && client->IsConnected())
				{
					client->Send(msg);
				}
				else
				{
					OnClientDisconnect(client);
					client.reset();
					m_DeqConnections.erase(std::remove(m_DeqConnections.begin(), m_DeqConnections.end(), nullptr), m_DeqConnections.end());
				}
			}
			
			void MessageAllClients(const Message& msg, Client ignoreClient = nullptr)
			{
				bool invalidClientExists = false;
				
				for (auto& client : m_DeqConnections)
				{
					if (client && client->IsConnected())
					{
						if (client != ignoreClient)
							client->Send(msg);
					}
					else
					{
						OnClientDisconnect(client);
						client.reset();
						invalidClientExists = true;
					}
					if (invalidClientExists)
					{
						m_DeqConnections.erase(std::remove(m_DeqConnections.begin(), m_DeqConnections.end(), nullptr), m_DeqConnections.end());
					}
				}
			}
			
			void Update(size_t maxMessages, bool wait = false)
			{
				if (wait) m_MessagesIn.wait();
				
				size_t messageCount = 0;
				while (messageCount < maxMessages && !m_MessagesIn.empty())
				{
					auto msg = m_MessagesIn.pop_front();
					OnMessage(msg.remote, msg.msg);
					messageCount++;
				}
			}
			
		protected:
			virtual bool OnClientConnect(Client client) = 0;
			virtual void OnClientDisconnect(Client client) = 0;
			virtual void OnMessage(Client client, Message& msg) = 0;
			
		protected:
			OwnedMessageQueue m_MessagesIn;
			std::deque<Client> m_DeqConnections;
			
			boost::asio::io_context m_AsioContext;
			std::thread m_ThreadContext;
			
			boost::asio::ip::tcp::acceptor m_AsioAcceptor;
			
			uint32_t m_IdCounter = 9999;
		};
	}
}
