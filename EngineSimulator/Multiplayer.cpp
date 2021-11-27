#include "pch.h"
#include <Shared.hpp>
#include <Client/Network.hpp>
#include <Client/Client.hpp>
#include "Multiplayer.hpp"


#include <raylib.h>
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
/// </summary>aimw
void MultiplayerNetwork::OnCreateMove()
{
	static bool first_time = false;
	static glm::vec3 pos = glm::vec3(0, 0, 0);
	static float lastHeading = 0.f;

	static uint32_t lastCreateMove = 0;

	
	std::uint32_t currentTickRate = (CurrentFrameTime - lastCreateMove);
	

	if (currentTickRate >= TIME_DELAY )
	{
		
		glm::vec3 velocitySinceLastUpdate = scene.localPosition - pos;

		LagRecord cmd;
		first_time = true;
		cmd.Position = scene.localPosition;
		cmd.cellIndex = scene.localCell->_index;

		cmd.ForwardSpeed = 0.f;

		cmd.Heading = 0.f;
		float HeadingDiff = scene.localHeading - lastHeading;
		

		lastHeading = scene.localHeading;
		cmd.Velocity = glm::vec3(0, 0, 0);
		cmd.tickCount = CurrentFrameTime;
		cmd.clientTickcount = CurrentFrameTime;

		
		if (velocitySinceLastUpdate != glm::vec3(0,0,0))
		{
			cmd.Velocity = glm::normalize(velocitySinceLastUpdate);
			cmd.ForwardSpeed = glm::distance(scene.localPosition, pos) / (float)TIME_DELAY;
		}
		if (HeadingDiff != 0.f)
		{
			cmd.HeadingShadow = scene.localHeading;
			cmd.Heading = HeadingDiff / (float)TIME_DELAY;
			cmd.HeadingSpeed = std::fabs(HeadingDiff) / (float)TIME_DELAY;
		}

		/*if (cmd.Velocity.length() > 0) {
			cmd.Velocity /= (float)currentTickRate;

			std::cout << "Sent Velocity " << cmd.Velocity.x << " y: " << cmd.Velocity.y << " z " << cmd.Velocity.z << std::endl;
		}*/

	
		if (IsKeyDown(KEY_SPACE))
			cmd.Jumping = true;

	//	std::cout << "LOCAL Velocity X: " << cmd.Velocity.x << " Y: " << cmd.Velocity.y << " Z: " << cmd.Velocity.z << std::endl;

		NetworkCreateLagRecord(cmd);

	
	//	std::cout << "Handle Local CreateMove tickrate  " << currentTickRate << std::endl;
		lastCreateMove = CurrentFrameTime;
		
	}
	pos = scene.localPosition;

}
void MultiplayerNetwork::OnEntityCreateMove(Entity* entity, NetworkPacket* packet)
{



}