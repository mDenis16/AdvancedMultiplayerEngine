
#include <Common.hpp>
#include <Shared.hpp>
#include "Network.h"
#include "Server.hpp"

Server::Server()
{
}

Server::~Server()
{
}

void Server::Initialize(std::uint32_t port)
{
	GameNetwork.Initialize(port, 500);


}


