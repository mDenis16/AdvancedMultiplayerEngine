#pragma once

class Player : public Ped
{
public:

#if SERVER
	std::vector<Entity*> inStream;

	Player(ENetPeer* sPeer, std::uint32_t Handle);
#endif

	Player(Entity& ent);

	Player(std::uint32_t Handle);
	~Player();

private:

};

