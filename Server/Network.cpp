#include <Common.hpp>
#include <Shared.hpp>
#include <enet\enet.h>

#include "Network.h"

void Network::Initialize(std::uint32_t port, int maxClients)
{
	ENetAddress addr;
	enet_address_set_host(&addr, "127.0.0.1");
	addr.port = port;
	HostListener = enet_host_create(&addr, maxClients, 1, 0, 0);
}
