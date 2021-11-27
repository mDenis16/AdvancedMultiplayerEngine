#include "pch.h"
#include "Multiplayer.hpp"

#include "nativeList.hpp"
#include "NativeThreadsafeExecutor.hpp"
#include <glm/gtx/rotate_vector.hpp>

//#include <VEngine\nativeList.hpp>
#include "Engine.hpp"
#include "Implementations.hpp"
#include <cmath>
#define nearbyPedArraySize 100
#define TIME_DELAY 70

auto genericMove = {"idle", "idle_intro", "idle_transition", "idle_turn_l_0", "idle_turn_l_-180", "idle_turn_l_-90", "idle_turn_r_0", "idle_turn_r_180", "idle_turn_r_90", "rstart_l_0", "rstart_l_-180", "rstart_l_-90", "rstart_r_0", "rstart_r_180", "rstart_r_90", "rstop_l", "rstop_quick_l", "rstop_quick_r", "rstop_r", "run", "run_down", "run_down_slope", "run_turn_180_l", "run_turn_180_r", "run_turn_l2", "run_turn_l3", "run_turn_r2", "run_turn_r3", "run_up", "run_up_slope", "runtowalk_left", "runtowalk_right", "sprint", "sprint_turn_l", "sprint_turn_r", "walk", "walk_down", "walk_down_slope", "walk_turn_180_l", "walk_turn_180_r", "walk_turn_l3", "walk_turn_l4", "walk_turn_r3", "walk_turn_r4", "walk_up", "walk_up_slope", "walktorun_left", "walktorun_right", "wstart_l_0", "wstart_l_-180", "wstart_l_-90", "wstart_r_0", "wstart_r_180", "wstart_r_90", "wstop_l_0", "wstop_l_180", "wstop_l_-180", "wstop_l_90", "wstop_l_-90", "wstop_quick_l_0", "wstop_quick_r_0", "wstop_r_0", "wstop_r_180", "wstop_r_-180", "wstop_r_90", "wstop_r_-90"};
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
	Job->Callbacks.resize(2);
	Job->Execute([Job, model, position]
		{       STREAMING::REQUEST_MODEL(model);
	while (!STREAMING::HAS_MODEL_LOADED(model))
		Engine::Wait(0);

	std::cout << "Model has loaded " << std::endl;

	auto ret = PED::CREATE_PED(1, model, position.x, position.y, position.z, 0, true, true);

	Job->Callbacks[0] = ret;
	Job->Callbacks[1] = Engine::HandleToPointer(ret);

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
	ent->MemoryHandle = Job->Callbacks[1];

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
float Lerp(float a, float b, float lerpFactor)
{
	float result = ((1.f - lerpFactor) * a) + (lerpFactor * b);
	return result;
}

float LerpDegrees(float a, float b, float lerpFactor) // Lerps from angle a to b (both between 0.f and 360.f), taking the shortest path
{
	float result;
	float diff = b - a;
	if (diff < -180.f)
	{
		// lerp upwards past 360
		b += 360.f;
		result = Lerp(a, b, lerpFactor);
		if (result >= 360.f)
		{
			result -= 360.f;
		}
	}
	else if (diff > 180.f)
	{
		// lerp downwards past 0
		b -= 360.f;
		result = Lerp(a, b, lerpFactor);
		if (result < 0.f)
		{
			result += 360.f;
		}
	}
	else
	{
		// straight lerp
		result = Lerp(a, b, lerpFactor);
	}

	return result;
}
void MultiplayerNetwork::OnEntityCreateMove(Entity* entity,  NetworkPacket* packet)
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
float angle_diff(float destAngle, float srcAngle) {
	float delta = 0.f;

	delta = fmodf(destAngle - srcAngle, 360.0f);
	if (destAngle > srcAngle) {
		if (delta >= 180)
			delta -= 360;
	}
	else {
		if (delta <= -180)
			delta += 360;
	}
	return delta;
}
/// <summary>
/// Called somewhere in game thread 20 ticks per second
/// </summary>
///	inline float TICKRATE = 70.f;
inline float TICKRATE = 70.f;

void MultiplayerNetwork::OnCreateMove()
{

	if (Connected) {
		static uint32_t lastCreateMove = 0;
		static float lastSentHeading = 0.f;
		static glm::vec3 lastSentPosition = glm::vec3(0, 0, 0);

		static glm::vec3 pos = glm::vec3(0, 0, 0);
		std::uint32_t currentTickRate = (CurrentFrameTime - lastCreateMove);


		if (currentTickRate >= TIME_DELAY)
		{
		
			lastCreateMove = CurrentFrameTime;

			auto localPed = PLAYER::GET_PLAYER_PED(0);
			if (localPed) {
				LagRecord cmd;
				

				auto position = ENTITY::GET_ENTITY_COORDS(localPed, true);
				glm::vec3 vecPosition = glm::vec3(position.x, position.y, position.z);

				auto rotation = ENTITY::GET_ENTITY_ROTATION(localPed, 2);
				auto velocity = ENTITY::GET_ENTITY_VELOCITY(localPed);
				float heading = ENTITY::GET_ENTITY_HEADING(localPed);
				glm::vec3 velocitySinceLastUpdate = vecPosition - pos;

				
				cmd.RawVelocity = glm::vec3(velocity.x, velocity.y, velocity.z);
				
				cmd.Position = vecPosition;

				cmd.ForwardSpeed = 0.f;

				cmd.Heading = 0.f;
				float HeadingDiff = heading - lastSentHeading;


				lastSentHeading = heading;
				cmd.Velocity = glm::vec3(0, 0, 0);
				cmd.tickCount = CurrentFrameTime;
				cmd.clientTickcount = CurrentFrameTime;


				if (velocitySinceLastUpdate != glm::vec3(0, 0, 0))
				{
					cmd.Velocity = glm::normalize(velocitySinceLastUpdate);
					cmd.ForwardSpeed = glm::distance(vecPosition, pos) / (float)TIME_DELAY;
				}
				if (HeadingDiff != 0.f)
				{
					cmd.HeadingShadow = heading;
					cmd.Heading = HeadingDiff / (float)TIME_DELAY;
					cmd.HeadingSpeed = std::fabs(HeadingDiff) / (float)TIME_DELAY;
				}


				cmd.tickCount = CurrentFrameTime;
				cmd.clientTickcount = CurrentFrameTime;

				cmd.cellIndex = grid->getCell(cmd.Position)->_index;
				printf(" LOCAL PED POSITION >> X: %f Y: %f Z: %f  \n", position.x, position.y, position.z);

				cmd.MoveType = 0;
				pos = vecPosition;
				if (BRAIN::IS_PED_WALKING(localPed)) {
					cmd.MoveType = 1;
				}
				else if (BRAIN::IS_PED_RUNNING(localPed)) {
					cmd.MoveType = 2;
				}
				else if (BRAIN::IS_PED_SPRINTING(localPed)) {
					cmd.MoveType = 3;
				}
				/*if (!STREAMING::HAS_ANIM_DICT_LOADED("move_m@generic"))
					STREAMING::REQUEST_ANIM_DICT("move_m@generic");

				for(auto& anim : genericMove)
				{
                  if (ENTITY::IS_ENTITY_PLAYING_ANIM(localPed, "move_m@generic", anim, false))
                  {
					  std::cout << "Found active animation " << anim <<  std::endl;
					  break;
                  }
				}*/

				NetworkCreateLagRecord(cmd);

			}


		}


		{
			while(!Multiplayer.streamLock.try_lock())
				Engine::Wait(1);

			auto localPed = PLAYER::GET_PLAYER_PED(0);
			if (localPed) {
				auto localPositionGta = ENTITY::GET_ENTITY_COORDS(localPed, true);

				auto localPosition = glm::vec3(localPositionGta.x, localPositionGta.y, localPositionGta.z);

				auto localVelocityGta = ENTITY::GET_ENTITY_VELOCITY(localPed);
				auto localVelocityRotationGta = ENTITY::GET_ENTITY_ROTATION_VELOCITY(localPed);

				auto localRotation = ENTITY::GET_ENTITY_ROTATION(localPed, 2);

				for (auto& entity : Multiplayer.StreamedEntities)
				{
					if (entity->GameHandle && ENTITY::DOES_ENTITY_EXIST(entity->GameHandle))
					{

					

						vector3 entityPos = ENTITY::GET_ENTITY_COORDS(entity->GameHandle, true);
						glm::vec3 Position = glm::vec3(entityPos.x, entityPos.y, entityPos.z);

						

						ENTITY::SET_ENTITY_NO_COLLISION_ENTITY(entity->GameHandle, localPed, false);
						ENTITY::SET_ENTITY_NO_COLLISION_ENTITY(localPed, entity->GameHandle, false);
						ENTITY::SET_ENTITY_COLLISION(entity->GameHandle, false, false);
						
	
						if (entity->Velocity != glm::vec3(0, 0, 0)) {

							const glm::vec3 averageDir = (entity->Velocity + (entity->ShadowPosition - entity->Position)) / 2.f;

							glm::vec3 Direction = glm::normalize(averageDir);
							glm::vec3 VelocityPerFrame = Direction * entity->EntitySpeed * TICKRATE;

							entity->Position += VelocityPerFrame;
							//ENTITY::SET_ENTITY_VELOCITY(entity->GameHandle, VelocityPerFrame.x, VelocityPerFrame.y, VelocityPerFrame.z);

							ENTITY::SET_ENTITY_COORDS_NO_OFFSET(entity->GameHandle, entity->Position.x, entity->Position.y, entity->Position.z, false, false, false);
						}
						
						//ENTITY::_SET_ENTITY_ANGULAR_VELOCITY(entity->GameHandle, lvec.x, lvec.y, lvec.z);

						//glm::vec3 lvc = glm::vec3(localVelocityGta.x, localVelocityGta.y, localVelocityGta.z);
						//pos = localPosition;

						//ENTITY::SET_ENTITY_COORDS_NO_OFFSET(entity->GameHandle, pos.x, pos.y, pos.z + 1.f, false, false, false);
						//ENTITY::SET_ENTITY_HEADING(entity->GameHandle, interpolatedHeading);


						//ENTITY::SET_ENTITY_HEADING(ent->GameHandle, headingInterpolated);

						//GRAPHICS::DRAW_LINE(Lerped.x, Lerped.y, Lerped.z, localPosition.x, localPosition.y, localPosition.z, 125, 255, 255, 255);
					
						//glm::vec3 predicted = Lerped + direction * 2.05f;

					/*	GRAPHICS::DRAW_LINE(predicted.x, predicted.y, predicted.z, localPosition.x, localPosition.y, localPosition.z, 255, 0, 0, 255);
						
							Player* player = (Player*)ent;

							std::cout << "player->PedMoveType << " << player->PedMoveType << std::endl;
							if (player->PedMoveType == OFMT_Still ) {
								BRAIN::TASK_STAND_STILL(ent->GameHandle, -1);

							}
							else if (player->PedMoveType == OFMT_Walking && !BRAIN::IS_PED_WALKING(localPed)) {
								BRAIN::TASK_GO_STRAIGHT_TO_COORD(ent->GameHandle, predicted.x, predicted.y, predicted.z, 1.0f, -1, ent->Heading, 0.0f);
								BRAIN::SET_PED_DESIRED_MOVE_BLEND_RATIO(ent->GameHandle, 1.0f);

							}
							else if (player->PedMoveType == OFMT_Sprinting && !BRAIN::IS_PED_SPRINTING(localPed)) {
								BRAIN::TASK_GO_STRAIGHT_TO_COORD(ent->GameHandle, predicted.x, predicted.y, predicted.z, 3.0f, -1, ent->Heading, 1.0f);
								PLAYER::SET_RUN_SPRINT_MULTIPLIER_FOR_PLAYER(ent->GameHandle, 1.49f);
								BRAIN::SET_PED_DESIRED_MOVE_BLEND_RATIO(ent->GameHandle, 3.0f);
							}
							else if (player->PedMoveType == OFMT_Running && !BRAIN::IS_PED_RUNNING(localPed)) {
								BRAIN::TASK_GO_STRAIGHT_TO_COORD(ent->GameHandle, predicted.x, predicted.y, predicted.z, 4.0f, -1, ent->Heading, 2.0f);
								BRAIN::SET_PED_DESIRED_MOVE_BLEND_RATIO(ent->GameHandle, 2.0f);

							}*/

							

					}
				}
			}

			Multiplayer.streamLock.unlock();
		}
	}
}


