#pragma once


class Network
{
public:
	Network(std::uint32_t port, int maxClients);
	~Network() {};
	Network() {};

	SAFE_PROP(std::unordered_map <std::uint32_t COMMA Entity*>, Entities);

	std::uint32_t lastHandle;
	std::uint32_t GenerateHandle() { return ++lastHandle;  }

	void OnClientConnect(ENetPeer* peer);

	void Initialize(std::uint32_t port, int maxClients);



	static void OnNetworkReleaseMessage(ENetPacket* packet) { delete (NetworkPacket*)packet->userData; }

	VARIABLE_THREAD(HandleFlow);
	VARIABLE_THREAD(HandleIncomingFlow);
	VARIABLE_THREAD(HandleOutgoingFlow);


	SAFE_PROP(std::queue<NetworkPacket*>, IncomingPackets);
	SAFE_PROP(std::queue<NetworkPacket*>, OutgoingPackets);

	SAFE_PROP(std::vector<Player*>, Players);

	


	ENetHost* HostListener = nullptr;
};

inline Network GameNetwork;
