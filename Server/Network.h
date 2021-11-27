#pragma once
#include "Streamer.hpp"


class Network
{
public:
	Network(std::uint32_t port, int maxClients);
	~Network() {};
	Network() {};

	SAFE_PROP(std::unordered_map <std::uint32_t COMMA Entity*>, Entities);

	std::uint32_t lastHandle = 55;
	std::uint32_t GenerateHandle() { return lastHandle++;  }

	void OnClientConnect(ENetPeer* peer);
	void OnClientDisconnect(ENetPeer* peer);

	void Initialize(std::uint32_t port, int maxClients);


	static void OnNetworkReleaseMessage(ENetPacket* packet) { delete (NetworkPacket*)packet->userData; }

	VARIABLE_THREAD(ProcessNetwork);
	VARIABLE_THREAD(NetworkThread);
	VARIABLE_THREAD(GameLogic);
	


	static inline std::uint32_t CurrentFrameTime;
	//SAFE_PROP(std::queue<NetworkPacket*>, IncomingPackets);
	//SAFE_PROP(std::queue<NetworkPacket*>, OutgoingPackets);

	std::condition_variable m_action_cond;
	std::mutex m_action_lock;
	std::queue<NetworkPacket*> m_actions;
	
	Streamer streamer;

	SAFE_PROP(ENetHost*, HostListener);
};

inline Network GameNetwork;
