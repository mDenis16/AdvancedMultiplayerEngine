#pragma once

enum class EntityType {
	ET_None,
	ET_Ped,
	ET_Player,
	ET_Vehicle
};

class Entity {
public:

	glm::vec3 Position;

	std::uint32_t Handle = 0;
#if CLIENT
	std::uint32_t GameHandle = 0;
#else 
	ENetPeer* m_peer = nullptr;
#endif

	std::uint32_t Dimension = 0;

	EntityType Type = EntityType::ET_None;

	bool IsNetworked = false;

	bool IsStreamed = false;

};