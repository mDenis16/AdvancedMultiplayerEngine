#include "pch.h"

#include <Client/Network.hpp>
#include <Client/Client.hpp>
#include "Multiplayer.hpp"
#include <EngineSimulator\SceneView.hpp>



void MultiplayerNetwork::EventDisconnect()
{

}

void MultiplayerNetwork::EventConnect()
{
	

	std::cout << "Connected to server." << std::endl;
}

void MultiplayerNetwork::HandlePacket(NetworkPacket* packet)
{

	//std::cout << "Handling packet type " << packet->Type << std::endl;
}

void MultiplayerNetwork::EntityStreamIn(Entity* ent)
{
	//std::cout << "Ent stream in" << std::endl;
}

void MultiplayerNetwork::EntityStreamOut(Entity* ent)
{
	//std::cout << "Ent stream out " << std::endl;
}

/// <summary>
/// Called somewhere in game thread 20 ticks per second
/// </summary>
void MultiplayerNetwork::OnCreateMove()
{
	static bool first_time = false;

	if (!first_time || ClockDuration(Clock::now()- lastCreateMove ).count() > 1000 )
	{
		ClientCmd cmd;
		first_time = true;
		cmd.Position = scene.localPosition;
		cmd.cellIndex = scene.localCell->_index;

		NetworkCreateMove(cmd);

		//std::cout << "Handle CreateMove  " << std::endl;
		lastCreateMove = Clock::now();
	}
	

}
