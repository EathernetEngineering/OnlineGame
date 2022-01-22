#include <common/CeeNet.hpp>
#include <cstdio>
#include <cstdlib>


enum class MessageTypes
{
	ServerAccept = 0,
	ServerDeny = 1,
	ServerPing = 2,
	MessageAll = 3,
	ServerMessage = 4
};

class Server : public cee::net::ServerInterface<MessageTypes>
{
public:
	Server(uint16_t port)
		: cee::net::ServerInterface<MessageTypes>(port)
	{
	}
	
	virtual ~Server()
	{
	}
	
protected:
	virtual bool OnClientConnect(Client client) override
	{
		Message msg;
		msg.header.id = MessageTypes::ServerAccept;
		client->Send(msg);
		return true;
	}
	
	virtual void OnClientDisconnect(Client client) override
	{
		printf("[SERVER] Removing client [%u]\n", client->GetId());
	}
	
	virtual void OnMessage(Client client, Message& msg) override
	{
		switch(msg.header.id)
		{
			case MessageTypes::ServerPing:
			{
				printf("[%u] Server Ping\n", client->GetId());
				client->Send(msg);
			}
			break;
			
			case MessageTypes::MessageAll:
			{
				printf("[%u] Message All\n", client->GetId());
				
				Message msg;
				msg.header.id = MessageTypes::ServerMessage;
				msg << client->GetId();
				MessageAllClients(msg, client);
			}
			break;
			
			default:
			break;
		}
	}
};

struct options
{
	uint16_t port;
};

int main(int argc, char* argv[])
{
	options opts { 0 };
	std::unique_ptr<Server> server;
	
	if (argc > 1)
	{
		for (uint32_t i = 1; i < argc; i+=2)
		{
			switch (argv[i][0])
			{
				case '-':
				{
					switch(argv[i][1])
					{
						case 'p':
						{
							if (atoi(argv[i+1]))
							{
								opts.port = atoi(argv[i+1]);
							}
						}
						break;
						
						case 'h':
						{
							printf("Usage: Server.86_64 [OPTIONS]\n\n"
								   "Options:\n"
								   "  -h       Print this information.\n"
								   "  -p PORT  select port to open server on.\n");
							goto finishApp;
						}
						break;
						
						default:
						{
							printf("Invalid option \"%s\". Too see options use \"-h\" for help.\n", argv[i]);
							goto finishApp;
						}
					}
				}
				break;
				
				default:
				{
					printf("Invalid option \"%s\". Too see options use \"-h\" for help.\n", argv[i]);
					goto finishApp;
				}
			}
		}
	}
	
	cee::net::SetErrorCallback([](cee::net::ErrorSeverity, const std::string& message)
	{
		printf("%s\n", message.c_str());
		fflush(stdout);
	});
	
	server.reset(new Server(opts.port ? opts.port : 60000));
	
	server->Start();
	
	while(1)
	{
		server->Update(-1, false);
	}
finishApp:
	
	return 0;
}

