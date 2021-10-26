#include "pch.h"
#include "Multiplayer.hpp"

#include "nativeList.hpp"
#include "NativeThreadsafeExecutor.hpp"
//#include <VEngine\nativeList.hpp>
#include "Engine.hpp"
#include "Implementations.hpp"
#include <cmath>
#define nearbyPedArraySize 100
inline uint32_t hashGet(const char* str)
{
	size_t len = strlen(str);
	size_t i = 0;
	uint32_t hash = 0;
	while (i != len) {
		hash += (char)tolower(str[i++]);
		hash += hash << 10;
		hash ^= hash >> 6;
	}
	hash += hash << 3;
	hash ^= hash >> 11;
	hash += hash << 15;
	return hash;
}
struct nearbyEnts {
	int size;//still 32 bit integer
	int64_t entities[nearbyPedArraySize];
};
inline uint32_t hashGet(const std::string& str)
{
	return hashGet(str.c_str());
}
void MultiplayerNetwork::EventDisconnect()
{

}

void MultiplayerNetwork::EventConnect()
{


	std::cout << "Connected to server." << std::endl;

	NATIVE_FUNCTION({
nearbyEnts arr;
	arr.size = nearbyPedArraySize;
	int ped = PLAYER::GET_PLAYER_PED(0);
	int size = PED::GET_PED_NEARBY_PEDS(ped, (int*)&arr, ped);
	for (int i = 0; i < size; i++) {

		int pedId = arr.entities[i];
		ENTITY::DELETE_ENTITY(&pedId);
	}
		}, 0);



	NATIVE_END()

		NativeThreadSafe::QueueJob* Job = new NativeThreadSafe::QueueJob();

	Job->Callbacks.resize(1);
	Job->Execute([Job]
		{

			auto model = GAMEPLAY::GET_HASH_KEY("mp_m_freemode_01");;
			STREAMING::REQUEST_MODEL(model);

			while (!STREAMING::HAS_MODEL_LOADED(model))
				Engine::Wait(1);
	
			PLAYER::SET_PLAYER_MODEL(PLAYER::GET_PLAYER_INDEX(), model);
			PED::SET_PED_DEFAULT_COMPONENT_VARIATION(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(PLAYER::GET_PLAYER_INDEX()));
		});
	Job->WaitforCallback();

	delete Job;


}

void MultiplayerNetwork::HandlePacket(NetworkPacket* packet)
{

	//std::cout << "Handling packet type " << packet->Type << std::endl;
}

void MultiplayerNetwork::EntityStreamIn(Entity* ent)
{



	auto model = hashGet("mp_m_freemode_01");
	ent->Model = model;

	std::cout << "EntityStreamIn >> " << ent->EntityHandle << " << " << std::endl;
	std::cout << "Entity model >> " << ent->Model << " << " << std::endl;
	std::cout << "Entity position >> " << ent->Position.x << ent->Position.y << ent->Position.z << " << " << std::endl;
	auto position = ent->Position;


	NativeThreadSafe::QueueJob* Job = new NativeThreadSafe::QueueJob();
	Job->Callbacks.resize(1);
	Job->Execute([Job, model, position]
		{       STREAMING::REQUEST_MODEL(model);
	while (!STREAMING::HAS_MODEL_LOADED(model))
		Engine::Wait(0);

	std::cout << "Model has loaded " << std::endl;

	auto ret = PED::CREATE_PED(1, model, position.x, position.y, position.z, 0, true, true);

	Job->Callbacks[0] = ret;

	while (!ENTITY::DOES_ENTITY_EXIST(ret))
		Engine::Wait(0);


	PED::SET_PED_CAN_BE_TARGETTED(ret, true);
	//	PED::SET_PED_CAN_BE_TARGETTED_BY_PLAYER(ret, _pGame->m_player.player->GameHandle, true);
	PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(ret, true);
	BRAIN::TASK_SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(ret, true);
	//	PED::SET_PED_RELATIONSHIP_GROUP_HASH(ret, _pGame->m_pedRelGroup);
//	ENTITY::FREEZE_ENTITY_POSITION(ret, true);
	PED::SET_PED_CAN_RAGDOLL(ret, true);
	PED::SET_PED_DEFAULT_COMPONENT_VARIATION(ret);
	PED::SET_PED_CAN_EVASIVE_DIVE(ret, false);
	PED::SET_PED_GET_OUT_UPSIDE_DOWN_VEHICLE(ret, false);
	PED::SET_PED_AS_ENEMY(ret, false);
	PED::SET_CAN_ATTACK_FRIENDLY(ret, true, false);
	PED::SET_PED_ALLOW_VEHICLES_OVERRIDE(ret, true);
	PED::SET_PED_CONFIG_FLAG(ret, 281, true); // PED_FLAG_NO_WRITHE
	//PHYSICS::ACTIVATE_PHYSICS(ret);

	std::cout << "spawned ped at " << position.x << " y " << position.y << " z " << position.z << std::endl;

		});
	Job->WaitforCallback();

	ent->GameHandle = Job->Callbacks[0];

	delete Job;

}

void MultiplayerNetwork::EntityStreamOut(Entity* ent)
{
	std::cout << "Ent stream out " << std::endl;

	auto handle = ent->GameHandle;

	if (handle) {
		NativeThreadSafe::QueueJob* Job = new NativeThreadSafe::QueueJob();
		Job->Callbacks.resize(2);
		Job->Execute([Job, handle]
			{
				int pedId = handle;

				if (ENTITY::DOES_ENTITY_EXIST(pedId))
					ENTITY::DELETE_ENTITY(&pedId);

			});

		Job->WaitforCallback();
		delete Job;

	}
}

void MultiplayerNetwork::OnEntityCreateMove(Entity* entity)
{
	if (entity->Type == EntityType::ET_Player)
	{

	}
}
float angleBetween(
	glm::vec3 a,
	glm::vec3 b,
	glm::vec3 origin
) {
	glm::vec3 da = glm::normalize(a - origin);
	glm::vec3 db = glm::normalize(b - origin);
	return glm::acos(glm::dot(da, db));
}
/// <summary>
/// Called somewhere in game thread 20 ticks per second
/// </summary>
void MultiplayerNetwork::OnCreateMove()
{
	if (Connected) {
		static bool first_time = false;

		if (!first_time || ClockDuration(Clock::now() - lastCreateMove).count() > 1000 / 20)
		{
			first_time = true;
			lastCreateMove = Clock::now();

			auto localPed = PLAYER::GET_PLAYER_PED(0);
			if (localPed) {
				LagRecord cmd;


				auto position = ENTITY::GET_ENTITY_COORDS(localPed, true);
				auto rotation = ENTITY::GET_ENTITY_ROTATION(localPed, 2);
				auto velocity = ENTITY::GET_ENTITY_VELOCITY(localPed);

				cmd.Position = glm::vec3(position.x, position.y, position.z);
				cmd.ForwardSpeed = ENTITY::GET_ENTITY_SPEED(localPed);
				cmd.Rotation = glm::vec3(rotation.x, rotation.y, rotation.z);;
				cmd.Velocity = glm::vec3(velocity.x, velocity.y, velocity.z);;
				cmd.Heading = ENTITY::GET_ENTITY_HEADING(localPed);


				cmd.cellIndex = grid->getCell(cmd.Position)->_index;
				printf(" LOCAL PED ROTATION >> X: %f Y: %f Z: %f  \n", cmd.Rotation.x, cmd.Rotation.y, cmd.Rotation.z);

				cmd.MoveType = 0;

				if (BRAIN::IS_PED_WALKING(localPed)) {
					cmd.MoveType = 1;
				}
				else if (BRAIN::IS_PED_RUNNING(localPed)) {
					cmd.MoveType = 2;
				}
				else if (BRAIN::IS_PED_SPRINTING(localPed)) {
					cmd.MoveType = 3;
				}


				NetworkCreateLagRecord(cmd);


			}


		}


		if (Multiplayer.streamLock.try_lock()) {
			auto localPed = PLAYER::GET_PLAYER_PED(0);
			if (localPed) {
				auto localPositionGta = ENTITY::GET_ENTITY_COORDS(localPed, true);

				auto localPosition = glm::vec3(localPositionGta.x, localPositionGta.y, localPositionGta.z);

				for (auto& ent : Multiplayer.StreamedEntities)
				{
					if (ent->GameHandle && ENTITY::DOES_ENTITY_EXIST(ent->GameHandle))
					{

						auto render = ent->Interpolation();

						//printf("REMOTE ROTATION >> X: %f Y: %f Z: %f  \n", render.Rotation.x, render.Rotation.y, render.Rotation.z);
						auto coordsgta = ENTITY::GET_ENTITY_COORDS(ent->GameHandle, true);

						auto coords = glm::vec3(coordsgta.x, coordsgta.y, coordsgta.z);
						std::cout << " updating streamed entity " << std::endl;
						glm::vec3 direction = render.Position - coords;

					//	GRAPHICS::DRAW_LINE(coords.x, coords.y, coords.z, render.Position.x, render.Position.y, render.Position.z, 255, 0, 0, 255);

						ENTITY::APPLY_FORCE_TO_ENTITY(ent->GameHandle, 1, direction.x, direction.y, direction.z, 0.0f, 0.0f, 0.0f, 0, 0, 1, 1, 0, 1);
					//	ENTITY::SET_ENTITY_QUATERNION(ent->GameHandle, render.Quaternion.x, render.Quaternion.y, render.Quaternion.z, render.Quaternion.w);

						if (ent->Type == EntityType::ET_Player) {
							auto player = (Player*)ent;
							float distance = glm::distance(coords, localPosition);

							if (player->lagRecords_lock.try_lock()) {


								for (int i = 0; i < player->lagRecords.size(); i += 2)
								{

									if (i > 0 && i + 1 < player->lagRecords.size()) {
										auto current = player->lagRecords.at(i);
										auto after = player->lagRecords.at(i + 1);
										if (current.MoveType == OFMT_Still)
											GRAPHICS::DRAW_LINE(current.Position.x, current.Position.y, current.Position.z, after.Position.x, after.Position.y, after.Position.z, 0, 0, 255, 255);
										else if (current.MoveType == OFMT_Walking)
											GRAPHICS::DRAW_LINE(current.Position.x, current.Position.y, current.Position.z, after.Position.x, after.Position.y, after.Position.z, 12, 215, 0, 255);
										else  if (current.MoveType == OFMT_Walking)
											GRAPHICS::DRAW_LINE(current.Position.x, current.Position.y, current.Position.z, after.Position.x, after.Position.y, after.Position.z, 150, 100, 15, 255);
										else  if (current.MoveType == OFMT_Sprinting)
											GRAPHICS::DRAW_LINE(current.Position.x, current.Position.y, current.Position.z, after.Position.x, after.Position.y, after.Position.z, 255, 0, 0, 255);
										else  if (current.MoveType == OFMT_Running)
											GRAPHICS::DRAW_LINE(current.Position.x, current.Position.y, current.Position.z, after.Position.x, after.Position.y, after.Position.z, 150, 25, 150, 255);

									}
								}

								if (player->lagRecords.size() > 2)
								{
									auto& cmd = player->lagRecords.back();
									auto& previous = player->lagRecords.at(player->lagRecords.size() - 2);
									auto distance = glm::distance(coords, cmd.Position);
									auto pedHeading = ENTITY::GET_ENTITY_HEADING(player->GameHandle);

									std::cout << "mata merge se fte " << std::endl;

									//if (distance <= 0.15f)
									//{

									//}
									//else if (distance <= 1.25f) // Walking
									//{
									//	/*Function.Call(Hash.TASK_GO_STRAIGHT_TO_COORD, Character, Position.X, Position.Y, Position.Z, 1.0f, -1, Character.Heading, 0.0f);
									//	Function.Call(Hash.SET_PED_DESIRED_MOVE_BLEND_RATIO, Character, 1.0f);*/

									//	BRAIN::TASK_GO_STRAIGHT_TO_COORD(player->GameHandle, cmd.Position.x, cmd.Position.y, cmd.Position.z, 1.0f, -1, pedHeading, 0.0f);
									//	BRAIN::SET_PED_DESIRED_MOVE_BLEND_RATIO(player->GameHandle, 1.0f);

									//}
									//else if (distance > 1.75f) // Sprinting
									//{
									//	/*Function.Call(Hash.TASK_GO_STRAIGHT_TO_COORD, Character, Position.X, Position.Y, Position.Z, 3.0f, -1, Character.Heading, 2.0f);
									//	Function.Call(Hash.SET_RUN_SPRINT_MULTIPLIER_FOR_PLAYER, Character, 1.49f);
									//	Function.Call(Hash.SET_PED_DESIRED_MOVE_BLEND_RATIO, Character, 3.0f);*/


									//	BRAIN::TASK_GO_STRAIGHT_TO_COORD(player->GameHandle, cmd.Position.x, cmd.Position.y, cmd.Position.z, 3.0f, -1, pedHeading, 2.0f);
									//	PLAYER::SET_RUN_SPRINT_MULTIPLIER_FOR_PLAYER(player->GameHandle, 1.49f);
									//	BRAIN::SET_PED_DESIRED_MOVE_BLEND_RATIO(player->GameHandle, 3.0f);

									//}
									//else // Running
									//{

									//	/*Function.Call(Hash.TASK_GO_STRAIGHT_TO_COORD, Character, Position.X, Position.Y, Position.Z, 4.0f, -1, Character.Heading, 1.0f);
									//	Function.Call(Hash.SET_PED_DESIRED_MOVE_BLEND_RATIO, Character, 2.0f);*/

									//	BRAIN::TASK_GO_STRAIGHT_TO_COORD(player->GameHandle, cmd.Position.x, cmd.Position.y, cmd.Position.z, 4.0f, -1, pedHeading, 1.0f);
									//	BRAIN::SET_PED_DESIRED_MOVE_BLEND_RATIO(player->GameHandle, 2.0f);
									//}

									glm::vec3 predicted = cmd.Position + (cmd.Position - previous.Position) + cmd.Velocity * 1.05f;
									

									if (distance > 0.15f)
									{
										if (cmd.MoveType == OFMT_Still) {
											BRAIN::TASK_STAND_STILL(player->GameHandle, -1);

										}
										else if (cmd.MoveType == OFMT_Walking) {
											BRAIN::TASK_GO_STRAIGHT_TO_COORD(player->GameHandle, predicted.x, predicted.y, predicted.z, 1.0f, -1, pedHeading, 0.0f);
											BRAIN::SET_PED_DESIRED_MOVE_BLEND_RATIO(player->GameHandle, 1.0f);

										}
										else if (cmd.MoveType == OFMT_Sprinting) {
											BRAIN::TASK_GO_STRAIGHT_TO_COORD(player->GameHandle, predicted.x, predicted.y, predicted.z, 3.0f, -1, pedHeading, 1.0f);
											PLAYER::SET_RUN_SPRINT_MULTIPLIER_FOR_PLAYER(player->GameHandle, 1.49f);
											BRAIN::SET_PED_DESIRED_MOVE_BLEND_RATIO(player->GameHandle, 3.0f);
										}
										else if (cmd.MoveType == OFMT_Running) {
											BRAIN::TASK_GO_STRAIGHT_TO_COORD(player->GameHandle, predicted.x, predicted.y, predicted.z, 4.0f, -1, pedHeading, 2.0f);
											BRAIN::SET_PED_DESIRED_MOVE_BLEND_RATIO(player->GameHandle, 2.0f);

										}
										GRAPHICS::DRAW_LINE(cmd.Position.x, cmd.Position.y, cmd.Position.z, predicted.x, predicted.y, predicted.z, 125, 255, 255, 255);
									}

									//MULTIPLAYER::GAME_PED::UpdatePedWalkingAnimation((Ped*)ent);

									
								}

								player->lagRecords_lock.unlock();
							}
						}
					}
				}
			}

			Multiplayer.streamLock.unlock();
		}

	}


}
