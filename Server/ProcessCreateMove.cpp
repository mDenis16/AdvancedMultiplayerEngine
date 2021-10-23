#include <Common.hpp>
#include <Shared.hpp>
#include <enet\enet.h>

#include "Network.h"

#include <functional>
#include "PacketHandler.hpp"
#include "ClientCmd/ClientCmd.hpp"

void PacketHandler::ProcessMovement(Player* player, NetworkPacket* packet)
{
	ClientCmd cmd;
	packet->Read(cmd.Position);
	packet->Read(cmd.Rotation);
	packet->Read(cmd.Direction);
	packet->Read(cmd.cellIndex);

	player->Position = cmd.Position;


	if (cmd.cellIndex != player->cellIndex)
	{

		auto& oldCell = GameNetwork.streamer.grid->cells[player->cellIndex];

		auto it = std::find(oldCell.Items.begin(), oldCell.Items.end(), player);
		if (it != oldCell.Items.end()) {
			//std::cout << "Removed entity from old cell." << std::endl;
			oldCell.Items.erase(it);
		}

		auto& newCell = GameNetwork.streamer.grid->cells[cmd.cellIndex];
		newCell.Items.push_back(player);


		player->cellIndex = cmd.cellIndex;
		
		GameNetwork.streamer.UpdateStream(player);
	}

	/*auto movePacket = new NetworkPacket(PlayerCreateMove, StreamRangeNetwork, player->Peer);
	movePacket->Write(cmd.Position);
	movePacket->Write(cmd.Rotation);
	movePacket->Write(cmd.Direction);
	movePacket->Send();*/

	
	//std::cout << "Processing movement "<< std::endl;
}


