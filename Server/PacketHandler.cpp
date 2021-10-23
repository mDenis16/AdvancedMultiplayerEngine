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
	case PlayerHandshake: {
		std::cout << "Processing Playerhandshake " << std::endl;
		auto packet = new NetworkPacket((int)PlayerHandshake, player->Peer, ENET_PACKET_FLAG_RELIABLE);

		packet->Write(999);

		packet->Send();
	}
		break;
	case PlayerCreateMove:
	{
		ProcessMovement(player, packet);
		
	}
	break;
	}
}


