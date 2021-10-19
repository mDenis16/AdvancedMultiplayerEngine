
#include <Common.hpp>
#include <enet\enet.h>
#include "Entity.hpp"
#include "Ped.hpp"
#include "Player.hpp"

#include <Shared.hpp>

#if SERVER
Player::Player(ENetPeer* sPeer, std::uint32_t Handle)
{
	this->Peer = sPeer;
	this->EntityHandle = Handle;
}
#endif

Player::Player(std::uint32_t Handle)
{
	this->EntityHandle = Handle;
}

Player::~Player()
{
}