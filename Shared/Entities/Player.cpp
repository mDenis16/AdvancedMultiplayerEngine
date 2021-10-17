
#include <Common.hpp>
#include <enet\enet.h>
#include "Entity.hpp"
#include "Ped.hpp"
#include "Player.hpp"

Player::Player(std::uint32_t Handle)
{
	this->Handle = Handle;
}

Player::~Player()
{
}