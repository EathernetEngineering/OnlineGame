#include "ClientLayer.h"
#include <common/CeeNet.hpp>
#include "ClientNetLayer.h"

#include <imgui.h>


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


