#include "pch.h"

#include <Client/Network.hpp>
#include <Client/Client.hpp>
#include "Multiplayer.hpp"
#include "MultiplayerClient.hpp"


void MultiplayerNetwork::EventDisconnect()
{
	
}

void MultiplayerNetwork::EventConnect()
{
}

void MultiplayerNetwork::HandlePacket(NetworkPacket* packet)
{
	packet->Packet->userData
}
