
#include <Common.hpp>
#include <enet\enet.h>
#include <Shared.hpp>


#if SERVER
Player::Player(ENetPeer* sPeer, std::uint32_t Handle)
{
	this->Peer = sPeer;
	this->EntityHandle = Handle;
	this->Type = EntityType::ET_Player;
}
#endif

Player::Player(EntityStruct& ent)
{
	this->EntityHandle = ent.EntityHandle;
	this->IsNetworked =  true;
	this->IsStreamed = true;
	this->Position = ent.Position;
	this->Type = EntityType::ET_Player;
}

void Player::CreateMove(NetworkPacket* packet)
{
	Ped::CreateMove(packet);
}

Player::Player(std::uint32_t Handle)
{
	this->EntityHandle = Handle;
	this->IsNetworked = true;
	this->IsStreamed = false;
	this->Type = EntityType::ET_Player;
}

Player::~Player()
{
}