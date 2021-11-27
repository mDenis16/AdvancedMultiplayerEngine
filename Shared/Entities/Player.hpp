#pragma once
#include "Network/NetworkPacket.hpp"



class Player : public Ped
{
public:

#if SERVER
	std::vector<Entity*> inStream;
	std::vector<Ped*> controlledPeds;

	Player(ENetPeer* sPeer, std::uint32_t Handle);
#endif

	Player(EntityStruct& ent);

	
	void CreateMove(NetworkPacket* packet) override;
	
	Player(std::uint32_t Handle);
	~Player();

private:

};

