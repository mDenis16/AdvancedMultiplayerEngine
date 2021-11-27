
#include <Common.hpp>
#include <Shared.hpp>
#include <enet\enet.h>
#include <enet\time.h>


#include "ClientCmd/LagRecord.hpp"


Ped::Ped(EntityStruct& ent)
{
	this->EntityHandle = ent.EntityHandle;
	this->IsNetworked = true;
	this->IsStreamed = true;
	this->Position = ent.Position;
	this->Type = EntityType::ET_Ped;
}

#if SERVER
#include "../Server/Network.h"
Ped::Ped(std::uint32_t Handle)
{
	this->EntityHandle = Handle;
	this->Peer = nullptr;
	this->Type = EntityType::ET_Ped;
}
void Ped::CreateMove(NetworkPacket* packet)
{
	LagRecord cmd = {};

	packet->Read<LagRecord>(cmd);


	//cmd.Velocity = cmd.Position - this->Position;
	this->Position = cmd.Position;


	std::cout << "Createmove tickrate " << Network::CurrentFrameTime - this->lastTickrate;
	this->lastTickrate = Network::CurrentFrameTime;

#if NO_LAITY
	if (this->Type == EntityType::ET_Player && this->Peer) {

		Player* player = static_cast<Player*>(this);
		if (!player->controlledPeds.empty()) {

			for (auto& dummy : player->controlledPeds) {

				packet->Current = 4;

				dummy->CreateMove(packet);
			}
		}


	}
	this->EntitiesInStreamRangeMutex.lock();

	auto movePacket = new NetworkPacket(PlayerCreateMove, this->EntitiesInStreamRange, 0, this->Peer, false);
	movePacket->Write(this->EntityHandle);
	movePacket->Write(cmd);

	movePacket->Send();

	this->EntitiesInStreamRangeMutex.unlock();

#endif

	if (cmd.cellIndex != this->cellIndex)
	{
		//GameNetwork.streamer.grid_lock.lock();
		//auto& oldCell = GameNetwork.streamer.grid->cells[this->cellIndex];

		//auto it = std::find(oldCell.Items.begin(), oldCell.Items.end(), this);
		//if (it != oldCell.Items.end()) {
		//	//std::cout << "Removed entity from old cell." << std::endl;
		//	oldCell.Items.erase(it);
		//}

		//auto& newCell = GameNetwork.streamer.grid->cells[cmd.cellIndex];
		//newCell.Items.push_back(this);

		this->oldCellIndex = this->cellIndex;
		this->cellIndex = cmd.cellIndex;

	
		GameNetwork.streamer.UpdateStream(this);
		
	}

	
}
#endif
#if CLIENT
#include "../Client/Network.hpp"

void Ped::CreateMove(NetworkPacket* packet)
{

	
	
	auto cmd = new LagRecord();

	packet->Read(*cmd);

	

	this->EntitySpeed = cmd->ForwardSpeed;
	this->HeadingSpeed = cmd->HeadingSpeed;
	 
	this->ShadowHeading = cmd->HeadingShadow;
	this->Velocity = cmd->Velocity;
	this->ShadowPosition = cmd->Position;
	this->RawVelocity = cmd->RawVelocity;
	this->HeadingPerTick = cmd->Heading;
	this->PedMoveType = cmd->MoveType;

	this->lastTickrate = Network::CurrentFrameTime - this->lastTickUpdate;
	this->lastTickUpdate = Network::CurrentFrameTime;
	
	cmd->tickCount = Network::CurrentFrameTime;

	std::cout << "Received Velocity " << cmd->Velocity.x << " y: " << cmd->Velocity.y << " z " << cmd->Velocity.z << std::endl;

	if (this->Velocity == glm::vec3(0,0,0))
	this->Position = cmd->Position;

}
#endif
