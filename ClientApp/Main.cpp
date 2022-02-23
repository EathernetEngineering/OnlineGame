#include <cstdio>
#include <common/CeeNet.hpp>

#include <Engine/Engine.h>

#include "ClientNetLayer.h"
#include "ClientInterfaceLayer.h"
#include "ClientDebugLayer.h"

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
	this->PushLayer(new ClientInterfaceLayer("ClientInterfaceLayer"));
	this->PushLayer(new ClientNetLayer("ClientNetLayer", m_Port));
#if defined(CEE_DEBUG)
	this->PushLayer(new ClientDebugLayer());
#endif
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
	
	app->Run();
	
finishApp:
	
	return 0;
}
