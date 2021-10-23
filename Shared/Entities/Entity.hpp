#pragma once
#include <enet/enet.h>

#include "Gridmap/Grid.hpp"
#include <mutex>


enum class EntityType {
	ET_None,
	ET_Ped,
	ET_Player,
	ET_Vehicle
};

class Entity {
public:

	GridCell<Entity*>* cell = nullptr;

	bool operator == (Entity* const& e)
	{
		return (EntityHandle == e->EntityHandle);
	}


	glm::vec3 Position;
	int oldCellIndex = 0;
	int cellIndex = 0;
	std::uint32_t EntityHandle = 0;
#if CLIENT
	std::uint32_t GameHandle = 0;
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


};