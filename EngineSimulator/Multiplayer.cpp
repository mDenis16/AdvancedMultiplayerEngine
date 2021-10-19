#include "pch.h"

#include <Client/Network.hpp>
#include <Client/Client.hpp>
#include "Multiplayer.hpp"



void MultiplayerNetwork::EventDisconnect()
{

}

void MultiplayerNetwork::EventConnect()
{
	std::cout << "Connected to server." << std::endl;
}

void MultiplayerNetwork::HandlePacket(NetworkPacket* packet)
{

}
