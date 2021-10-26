#include "pch.h"
#include <Shared.hpp>
#include <Client/Network.hpp>
#include <Client/Client.hpp>
#include "Multiplayer.hpp"


#include <EngineSimulator\SceneView.hpp>

void MultiplayerNetwork::EventDisconnect()
{

}

void MultiplayerNetwork::EventConnect()
{
	
	scene.localPosition = glm::vec3(-763.4022, 7.327758, 40.59016);

	std::cout << "Connected to server." << std::endl;
}

void MultiplayerNetwork::HandlePacket(NetworkPacket* packet)
{

	//std::cout << "Handling packet type " << packet->Type << std::endl;
}

void MultiplayerNetwork::EntityStreamIn(Entity* ent)
{
//	scene.localPosition = ent->Position;

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
	static glm::vec3 pos = glm::vec3(0, 0, 0);

	if (!first_time || ClockDuration(Clock::now()- lastCreateMove ).count() > 1000 / 5 )
	{
		LagRecord cmd;
		first_time = true;
		cmd.Position = scene.localPosition;
		cmd.cellIndex = scene.localCell->_index;
		cmd.ForwardSpeed = 3.f;
		cmd.Velocity = (cmd.Position - pos);

		std::cout << "LOCAL Position ped X: " << cmd.Position.x << " Y: " << cmd.Position.y << " Z: " << cmd.Position.z << std::endl;

		NetworkCreateLagRecord(cmd);

		pos = scene.localPosition;
		//std::cout << "Handle CreateMove  " << std::endl;
		lastCreateMove = Clock::now();
	}
	

}
void MultiplayerNetwork::OnEntityCreateMove(Entity* entity)
{
	
}