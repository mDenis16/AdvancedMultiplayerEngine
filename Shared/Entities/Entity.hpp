#pragma once
#include <enet/enet.h>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/epsilon.hpp>
#include "Gridmap/Grid.hpp"
#include <mutex>
#include <deque>



enum class EntityType {
	ET_None,
	ET_Ped,
	ET_Player,
	ET_Vehicle
};

struct EntityStruct
{
	EntityType Type = EntityType::ET_None;
	std::uint32_t EntityHandle = 0;
	glm::vec3 Position;
};
struct EntityRenderData
{
	glm::vec3 Position = glm::vec3(0,0,0);
	glm::vec3 Rotation = glm::vec3(0, 0, 0);
	glm::vec3 Velocity = glm::vec3(0, 0, 0);
	glm::vec3 VelocityRotation = glm::vec3(0, 0, 0);
	glm::quat Quaternion = glm::quat(0,0,0,0);
	glm::quat AngularVelocity = glm::quat(0, 0, 0, 0);

	float Heading = 0.f;
	float ForwardSpeed = 0.f;


};
class Entity {
public:
	float Progress = 0.f;
	struct Interpolator
	{
		struct
		{
			glm::vec3      Start;
			glm::vec3      Target;
			glm::vec3      Error;
			float         LastAlpha;
			unsigned int StartTime;
			unsigned int FinishTime;
		} Position;

	};
	std::uint32_t TimeWhenVelocityStopped = 0;

	std::uint32_t lastTickUpdate = 0;
	int Frames = 0;
	Interpolator interpolation;

	unsigned int LastSyncReceived = 0;
	GridCell<Entity*>* cell = nullptr;

	bool operator == (Entity* const& e)
	{
		return (EntityHandle == e->EntityHandle);
	}
	
	bool operator == (std::uint32_t eHandle)
	{
		return (EntityHandle == eHandle);
	}
	glm::vec3 PreviousVelocity;
	glm::vec3 RawVelocity;
	glm::vec3 Velocity;
	glm::vec3 Position;
	glm::vec3 lastVelocity;
	glm::vec3 ShadowPosition = glm::vec3(0, 0, 0);
	float Heading = 0.f;
	float ShadowHeading = 0.f;
	float PreviousHeading = 0.f;
	float HeadingPerTick = 0.f;

	std::uint32_t Model;
//
	int aaa = 5;
	SAFE_PROP(std::deque<LagRecord*>, lagRecords);
	SAFE_PROP(std::deque<LagRecord>, renderRecords);
	int oldCellIndex = 0;
	int cellIndex = 0;
	std::uint32_t EntityHandle = 0;
#if CLIENT
	int GameHandle = 0;
	std::uint64_t MemoryHandle;
#else 
	ENetPeer* Peer = nullptr;
#endif

	std::uint32_t Dimension = 0;

	EntityType Type = EntityType::ET_None;

	bool IsNetworked = false;
	float EntitySpeed = 0.f;
	float HeadingSpeed = 0.f;

	bool IsStreamed = false;

	std::mutex EntitiesInStreamRangeMutex;
	std::vector<Entity*> EntitiesInStreamRange;
	

	std::vector<unsigned char> Serialize();

	virtual void CreateMove(NetworkPacket* packet) = 0;
	EntityRenderData Interpolation();

	std::atomic<bool> bUpdate{ false };


	bool ShouldDelay();

	void Interp(float dt);


	glm::vec3 lastPosition = glm::vec3(0, 0, 0);
	glm::vec3 Interpolation(float time);

	 std::uint32_t lastPreUpdate = 0;
	std::uint32_t lastUpdate = 0;
	std::uint32_t   shadowUpdate = 0;
	std::uint32_t lastTickrate = 0;

	std::uint32_t previousTickcount = 0;




};