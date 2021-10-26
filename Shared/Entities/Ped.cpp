
#include <Common.hpp>
#include <Shared.hpp>
#include <enet\enet.h>


#include "ClientCmd/LagRecord.hpp"

#if SERVER
#include "../Server/Network.h"
void Ped::CreateMove(NetworkPacket* packet)
{
	LagRecord cmd = {};

	packet->Read<LagRecord>(cmd);
	

	this->Position = cmd.Position;


	if (cmd.cellIndex != this->cellIndex)
	{

		auto& oldCell = GameNetwork.streamer.grid->cells[this->cellIndex];

		auto it = std::find(oldCell.Items.begin(), oldCell.Items.end(), this);
		if (it != oldCell.Items.end()) {
			//std::cout << "Removed entity from old cell." << std::endl;
			oldCell.Items.erase(it);
		}

		auto& newCell = GameNetwork.streamer.grid->cells[cmd.cellIndex];
		newCell.Items.push_back(this);

		this->oldCellIndex = this->cellIndex;
		this->cellIndex = cmd.cellIndex;

		GameNetwork.streamer.UpdateStream(this);
	}

	auto movePacket = new NetworkPacket(PlayerCreateMove, this->EntitiesInStreamRange, 0, this->Peer, false);
	movePacket->Write(this->EntityHandle);
	movePacket->Write(cmd);

	movePacket->Send();
}
#endif
#if CLIENT
#include "../Client/Network.hpp"
void Ped::CreateMove(NetworkPacket* packet)
{
	auto cmd = LagRecord();
	
	packet->Read(cmd);


	cmd.simulationTime = Clock::now();
	this->ForwardSpeed = cmd.ForwardSpeed;
	this->Position = cmd.Position;
	this->Velocity = cmd.Velocity;
	this->Rotation = cmd.Rotation;
	this->PedMoveType = cmd.MoveType;


	//std::cout << "Received Rotation ped X: " << cmd.Rotation.x << " Y: " << cmd.Rotation.y << " Z: " << cmd.Rotation.z << std::endl;

	SAFE_MODIFY(this->lagRecords)

		this->lagRecords.push_back(cmd);


	/*while (this->lagRecords.size() > 6)
		this->lagRecords.pop_front();*/


}
#endif
