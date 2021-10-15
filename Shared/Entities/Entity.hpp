#pragma once

class Entity {
public:
	std::uint32_t Id;
#if CLIENT
	std::uint32_t Handle;
#else 
	ENetPeer* m_peer = nullptr;
#endif
};