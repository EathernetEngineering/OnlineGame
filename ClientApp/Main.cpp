#include <cstdio>
#include <common/CeeNet.hpp>

#include <termios.h>
#include <unistd.h>

enum class MessageTypes
{
	ServerAccept = 0,
	ServerDeny = 1,
	ServerPing = 2,
	MessageAll = 3,
	ServerMessage = 4
};

struct options
{
	uint16_t port;
};

class Client : public cee::net::ClientInterface<MessageTypes>
{
public:
	Client() = default;
	virtual ~Client() = default;
	
public:
	void PingServer()
	{
		Message msg;
		msg.header.id = MessageTypes::ServerPing;
		std::chrono::system_clock::time_point startTime = std::chrono::system_clock::now();
		msg << startTime;
		Send(msg);
	}
	
	void MessageAll()
	{
		Message msg;
		msg.header.id = MessageTypes::MessageAll;
		Send(msg);
	}
};

int main(int argc, char** argv)
{
	options opts { 0 };
	std::unique_ptr<Client> client;
	bool quit = false;
	
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
							printf("Usage: Client.86_64 [OPTIONS]\n\n"
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
	
	client.reset(new Client);
	
	client->Connect("127.0.0.1", (opts.port ? opts.port : 60000));
	
	termios originalTerminalAtrrib;
	
	{
		termios newTerminalAtrrib;
		
		tcgetattr(fileno(stdin), &originalTerminalAtrrib);
		memcpy(&newTerminalAtrrib, &originalTerminalAtrrib, sizeof(termios));
		
		newTerminalAtrrib.c_lflag &= ~(ECHO|ICANON);
		newTerminalAtrrib.c_cc[VTIME] = 0;
		newTerminalAtrrib.c_cc[VMIN] = 0;
		tcsetattr(fileno(stdin), TCSANOW, &newTerminalAtrrib);
	}
	
	if (!client->IsConnected()) goto finishApp;
	
	while (!quit) {
	if (client->IsConnected())
		{
			if (!client->Incoming().empty())
			{
				auto msg = client->Incoming().pop_front().msg;
				switch(msg.header.id)
				{
					case MessageTypes::ServerAccept:
					{
						printf("Server Accept Confrimed\n");
					}
					break;
					
					case MessageTypes::ServerPing:
					{
						std::chrono::system_clock::time_point endTime = std::chrono::system_clock::now();
						std::chrono::system_clock::time_point startTime;
						msg >> startTime;
						printf("Ping: %fms\n", std::chrono::duration<float>(endTime - startTime).count());
					}
					break;
					
					case MessageTypes::ServerMessage:
					{
						uint32_t clientId;
						msg >> clientId;
						printf("Hello from [%u]\n", clientId);
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
				client->PingServer();
			}
			if (keyDown == 'a')
			{
				client->MessageAll();
			}
			if (keyDown == 'q')
			{
				quit = true;
			}
		}
		else
		{
			printf("Server down!\n");
			quit = true;
		}
	}
	
finishApp:
	tcsetattr(fileno(stdin), TCSANOW, &originalTerminalAtrrib);
	
	return 0;
}
