#pragma once


class Network
{
public:
	Network(std::uint32_t port, int maxClients);
	~Network() {};
	Network() {};
	std::unordered_map <std::uint32_t, Entity*> EntityList;


	void Initialize(std::uint32_t port, int maxClients);

	

	VARIABLE_THREAD(HandleIncomingFlow);
	VARIABLE_THREAD(HandleOutgoingFlow);


	SAFE_PROP(std::queue<NetworkPacket*>, IncomingPackets);
	SAFE_PROP(std::queue<NetworkPacket*>, OutgoingPackets);

	SAFE_PROP(std::vector<Player*>, Entities);

	


	ENetHost* HostListener = nullptr;
};

