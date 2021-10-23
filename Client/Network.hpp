#pragma once
#include <Shared.hpp>

#include "ClientCmd/ClientCmd.hpp"


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

	void Disconnect();

	virtual void EventDisconnect() = 0;;
	virtual void EventConnect() = 0;;
	virtual void HandlePacket(NetworkPacket* packet) = 0;;
	virtual void EntityStreamIn(Entity* ent) = 0;
	virtual void EntityStreamOut(Entity* ent) = 0;



	void NetworkCreateMove(ClientCmd& cmd);
	SAFE_PROP(std::queue<NetworkPacket*>, IncomingPackets);
	SAFE_PROP(std::queue<NetworkPacket*>, OutgoingPackets);

	ENetHost* Local = nullptr;
	ENetPeer* Peer = nullptr;

	SAFE_PROP(std::unordered_map <std::uint32_t COMMA Entity*>, Entities);

	std::vector<Entity*> StreamedEntities;
	std::atomic<bool> Connected{ false };

	int NetworkTickrate = 30;

	Grid<Entity*>* grid = nullptr;

	void ProcessStreamOut(NetworkPacket* packet);

	void ProcessStream(NetworkPacket* packet);

};
