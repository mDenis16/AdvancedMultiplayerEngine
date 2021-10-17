#pragma once
#include <Shared.hpp>



class Network {
public:
	Network();
	~Network();

	void HandleFlow();

	VARIABLE_THREAD(HandleIncomingFlow);
	VARIABLE_THREAD(HandleOutgoingFlow);


	void SendHandshake();

	static void OnNetworkReleaseMessage(ENetPacket* packet) { delete (NetworkPacket*)packet->userData; }

	void Connect(std::string& hostname, std::uint16_t port);

	virtual void EventDisconnect();
	virtual void EventConnect();


	SAFE_PROP(std::queue<NetworkPacket*>, IncomingPackets);
	SAFE_PROP(std::queue<NetworkPacket*>, OutgoingPackets);

	ENetHost* Local = nullptr;
	ENetPeer* Peer = nullptr;
	bool Connected = false;
	
};