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
		class ClientInterface
		{
		public:
			using Message = message<T>;
			using OwnedMessage = owned_message<T>;
			using MessageQueue = TsQueue<Message>;
			using OwnedMessageQueue = TsQueue<OwnedMessage>;
			using ConnectionType = Connection<T>;
			
		public:
			ClientInterface()
			{
			}
			
			virtual ~ClientInterface()
			{
				Disconnect();
			}
			
		public:
			bool Connect(const std::string& host, const uint16_t port)
			{
				try
				{
					boost::asio::ip::tcp::resolver resolver(m_Context);
					boost::asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(host, std::to_string(port));
					
					m_Connection = std::make_unique<ConnectionType>(ConnectionType::owner::client, m_Context, boost::asio::ip::tcp::socket(m_Context), m_MessagesIn);
					
					m_Connection->ConnectToServer(endpoints);
					m_ThreadContext = std::thread([this](){ m_Context.run(); });
				} 
				catch (std::exception& e)
				{
					fprintf(stderr, "Client Exception: %s\n", e.what());
					return false;
				}
				catch (boost::exception& e)
				{
					fprintf(stderr, "Boost Client Exception\n");
					return false;
				}
				return true;
			}
			
			void Disconnect()
			{
				if (IsConnected())
				{
					m_Connection->Disconnect();
				}
				
				m_Context.stop();
				if (m_ThreadContext.joinable())
					m_ThreadContext.join();
				
				m_Connection.release();
			}
			
			bool IsConnected()
			{
				if (m_Connection)
					return m_Connection->IsConnected();
				else
					return false;
			}
			
		public:
			void Send(const Message& msg)
			{
				if (IsConnected())
					m_Connection->Send(msg);
			}
			
			OwnedMessageQueue& Incoming()
			{
				return m_MessagesIn;
			}
			
		protected:
			boost::asio::io_context m_Context;
			std::thread m_ThreadContext;
			std::unique_ptr<ConnectionType> m_Connection;
			
		private:
			OwnedMessageQueue m_MessagesIn;
		};
	}
}
