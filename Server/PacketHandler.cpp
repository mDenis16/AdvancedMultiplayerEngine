#include <Common.hpp>
#include <Shared.hpp>
#include <enet\enet.h>

#include "Network.h"

#include <functional>
#include "PacketHandler.hpp"

void PacketHandler::Process(Player* player, NetworkPacket* packet)
{
	switch (packet->Type)
	{
	case PlayerHandshake:
		std::cout << "Processing Playerhandshake " << std::endl;
		break;
	}
}


