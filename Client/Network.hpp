#pragma once
#include <Shared.hpp>



class Network {
public:
	Network();
	~Network();

	VARIABLE_THREAD(HandleFlow);
	VARIABLE_THREAD(HandleIncomingFlow);
	VARIABLE_THREAD(HandleOutgoingFlow);


	void SendHandshake();

	static void OnNetworkReleaseMessage(ENetPacket* packet) { delete (NetworkPacket*)packet->userData; }

	void Connect(std::string hostname, std::uint16_t port);

	virtual void EventDisconnect() = 0;;
	virtual void EventConnect() = 0;;
	virtual void HandlePacket(NetworkPacket* packet) = 0;;



	SAFE_PROP(std::queue<NetworkPacket*>, IncomingPackets);
	SAFE_PROP(std::queue<NetworkPacket*>, OutgoingPackets);

	ENetHost* Local = nullptr;
	ENetPeer* Peer = nullptr;
	
	std::atomic<bool> Connected{ false };

	int NetworkTickrate = 30;
	
};
