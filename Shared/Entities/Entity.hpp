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
	glm::quat Quaternion = glm::quat(0,0,0,0);

	float Heading = 0.f;
};
class Entity {
public:

	GridCell<Entity*>* cell = nullptr;

	bool operator == (Entity* const& e)
	{
		return (EntityHandle == e->EntityHandle);
	}
	
	bool operator == (std::uint32_t eHandle)
	{
		return (EntityHandle == eHandle);
	}

	glm::vec3 Velocity;
	glm::vec3 Position;
	glm::vec3 Rotation;
	float Heading = 0.f;
	glm::vec3 RenderPosition;
	std::uint32_t Model;


	SAFE_PROP(std::deque<LagRecord>, lagRecords);
	int oldCellIndex = 0;
	int cellIndex = 0;
	std::uint32_t EntityHandle = 0;
#if CLIENT
	int GameHandle = 0;
#else 
	ENetPeer* Peer = nullptr;
#endif

	std::uint32_t Dimension = 0;

	EntityType Type = EntityType::ET_None;

	bool IsNetworked = false;

	bool IsStreamed = false;

	std::mutex EntitiesInStreamRangeMutex;
	std::vector<Entity*> EntitiesInStreamRange;
	

	std::vector<unsigned char> Serialize();

	virtual void CreateMove(NetworkPacket* packet) = 0;
	EntityRenderData Interpolation();

	EntityRenderData renderData = { glm::vec3(0,0,0), glm::vec3(0,0,0), glm::vec3(0,0,0) };

};