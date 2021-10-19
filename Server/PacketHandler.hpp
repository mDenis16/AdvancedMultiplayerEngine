#pragma once
#include "Entities/Player.hpp"

namespace PacketHandler
{
	void Process(Player* player, NetworkPacket* packet);
}
