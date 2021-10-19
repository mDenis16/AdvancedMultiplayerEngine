#pragma once

class Player : public Ped
{
public:

#if SERVER
	
	Player(ENetPeer* sPeer, std::uint32_t Handle);
#endif

	Player(std::uint32_t Handle);
	~Player();

private:

};

