#pragma once
#include <common/Common.hpp>
#include <common/TsQueue.hpp>
#include <common/Message.hpp>

namespace cee
{
	namespace net
	{
		template<typename T>
		class Connection : public std::enable_shared_from_this<Connection<T>>
		{
		public:
			enum class owner
			{
				server,
				client
			};
			
			using Message = message<T>;
			using OwnedMessage = owned_message<T>;
			using MessageQueue = TsQueue<Message>;
			using OwnedMessageQueue = TsQueue<OwnedMessage>;
			
			
		public:
			Connection(owner parent, boost::asio::io_context& asioContext, boost::asio::ip::tcp::socket socket, OwnedMessageQueue& MessagesIn)
			: m_Socket(std::move(socket)), m_AsioContext(asioContext), m_MessagesIn(MessagesIn)
			{
				m_OwnerType = parent;
			}
			
			virtual ~Connection() {}
			
			uint32_t GetId() { return m_Id; }
			
		public:
			void ConnectToClient(uint32_t uid = 0)
			{
				if (m_OwnerType == owner::server)
				{
					if (m_Socket.is_open())
					{
						m_Id = uid;
						ReadHeader();
					}
				}
			}
			
			void ConnectToServer(const boost::asio::ip::tcp::resolver::results_type& endpoints)
			{
				if (m_OwnerType == owner::client)
				{
					boost::asio::async_connect(m_Socket, endpoints,
											   [this](boost::system::error_code ec, boost::asio::ip::tcp::endpoint endpoint)
											   {
												   if (!ec)
												   {
													   ReadHeader();
												   }
											});
				}
			}
			
			void Disconnect()
			{
				if (IsConnected())
					boost::asio::post(m_AsioContext, [this]() { m_Socket.close(); });
			}
			
			bool IsConnected() { return m_Socket.is_open(); }
			
			void StartListening()
			{
			}
			
		public:
			void Send(const Message& msg)
			{
				boost::asio::post(m_AsioContext,
								  [this, msg]()
								  {
									  bool WritingMessage = !m_MessagesOut.empty();
									  m_MessagesOut.push_back(msg);
									  if(!WritingMessage)
										  WriteHeader();
								});
			}
			
		private:
			void WriteHeader()
			{
				boost::asio::async_write(m_Socket, boost::asio::buffer(&m_MessagesOut.front().header, sizeof(message_header<T>)),
										 [this](boost::system::error_code ec, std::size_t length)
										 {
											 if (!ec)
											 {
												 if (m_MessagesOut.front().body.size() > 0)
													 WriteBody();
												 else
												 {
													 m_MessagesOut.pop_front();
												 if (!m_MessagesOut.empty())
													 {
														 WriteHeader();
													 }
												 }
											 }
											 else
											 {
												 printf("[%u] WriteHeader() Failed!\n", m_Id);
												 m_Socket.close();
											 }
										});
			}
			
			void WriteBody()
			{
				boost::asio::async_write(m_Socket, boost::asio::buffer(m_MessagesOut.front().body.data(), m_MessagesOut.front().body.size()),
										 [this](boost::system::error_code ec, std::size_t length)
										 {
											 if (!ec)
											 {
												 m_MessagesOut.pop_front();
												 
												 if (!m_MessagesOut.empty())
												 {
													 WriteHeader();
												 }
											 }
											 else
											 {
												 printf("[%u] WriteBody() Failed!\n", m_Id);
												 m_Socket.close();
											 }
										});
			}
			
			void ReadHeader()
			{
				boost::asio::async_read(m_Socket, boost::asio::buffer(&m_MsgTempIn.header, sizeof(message_header<T>)),
																	  [this](boost::system::error_code ec, std::size_t length)
																	  {
																		  if (!ec)
																		  {
																			  if (m_MsgTempIn.header.size > 0)
																			  {
																				  m_MsgTempIn.body.resize(m_MsgTempIn.header.size);
																				  ReadBody();
																			  }
																			  else
																			  {
																				  AddToIncomingMessageQueue();
																			  }
																		  }
																		  else
																		  {
																			  printf("[%u] ReadHeader() Failed!\n", m_Id);
																			  m_Socket.close();
																		  }
																	});
			}
			
			void ReadBody()
			{
				boost::asio::async_read(m_Socket, boost::asio::buffer(m_MsgTempIn.body.data(), m_MsgTempIn.body.size()),
																	  [this](boost::system::error_code ec, std::size_t length)
																	  {
																		  if (!ec)
																		  {
																			  AddToIncomingMessageQueue();
																		  }
																		  else
																		  {
																			  printf("[%u] ReadBody() Failed!\n", m_Id);
																			  m_Socket.close();
																		  }
																	});
			}
			
			void AddToIncomingMessageQueue()
			{
				if (m_OwnerType == owner::server)
					m_MessagesIn.push_back({ this->shared_from_this(), m_MsgTempIn });
				else
					m_MessagesIn.push_back({ nullptr, m_MsgTempIn });
				
				ReadHeader();
			}
			
		protected:
			boost::asio::ip::tcp::socket m_Socket;
			boost::asio::io_context& m_AsioContext;
			
			MessageQueue m_MessagesOut;
			OwnedMessageQueue& m_MessagesIn;

			
			Message m_MsgTempIn;
			owner m_OwnerType = owner::server;
			
			uint32_t m_Id = 0;
		};
	}
}
