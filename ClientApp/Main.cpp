#include <cstdio>
#include <common/CeeNet.hpp>

#include <Engine/Engine.h>

#include "ClientLayer.h"

#include <termios.h>
#include <unistd.h>

struct options
{
	uint16_t port;
};

class ClientApplication : public cee::engine::Application
{
public:
	ClientApplication(uint16_t port = 60000)
		: m_Port(port)
{
	PushLayer(new ClientLayer("Client App", port));
}
	
public:
	virtual void OnAwake() override
	{
	}
	
	virtual void OnStart() override
	{
	}
	
private:
	uint16_t m_Port;
};

int main(int argc, char** argv)
{
	options opts { 0 };
	termios originalTerminalAtrrib;
	
	std::unique_ptr<cee::engine::Application> app;
	
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
	
	app.reset(new ClientApplication(opts.port ? opts.port : 60000));
	
	{
		termios newTerminalAtrrib;
		
		tcgetattr(fileno(stdin), &originalTerminalAtrrib);
		memcpy(&newTerminalAtrrib, &originalTerminalAtrrib, sizeof(termios));
		
		newTerminalAtrrib.c_lflag &= ~(ECHO|ICANON);
		newTerminalAtrrib.c_cc[VTIME] = 0;
		newTerminalAtrrib.c_cc[VMIN] = 0;
		tcsetattr(fileno(stdin), TCSANOW, &newTerminalAtrrib);
	}
	
	app->Run();
	
	tcsetattr(fileno(stdin), TCSANOW, &originalTerminalAtrrib);
	
finishApp:
	
	return 0;
}
