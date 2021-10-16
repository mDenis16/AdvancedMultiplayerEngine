#pragma once


class Network
{
public:
	Network(std::uint32_t port, int maxClients);
	~Network() {};
	Network() {};
	std::unordered_map <std::uint32_t, Entity*> EntityList;


	void Initialize(std::uint32_t port, int maxClients);

private:

	SAFE_PROP(std::queue<NetworkPacket*>, IncomingPackets);
	SAFE_PROP(std::queue<NetworkPacket*>, OutgoingPackets);
	SAFE_PROP(std::vector<Player*>, Entities);

	void QueueExecutor();
	VARIABLE_THREAD(Executor);

	ENetHost* HostListener = nullptr;
};

