#pragma once
#include <Shared.hpp>

#include "ClientCmd/LagRecord.hpp"


class Network {
public:
	//Network();
	//~Network();


	VARIABLE_THREAD(ProcessNetwork);
	VARIABLE_THREAD(NetworkThread);
	VARIABLE_THREAD(HandleOutgoingFlow);


	void Initialize();

	void Destroy();

	void SendHandshake();

	static void OnNetworkReleaseMessage(ENetPacket* packet) { delete (NetworkPacket*)packet->userData; }

	void Connect(std::string hostname, std::uint16_t port);

	void Disconnect();

	virtual void EventDisconnect() = 0;
	virtual void EventConnect() = 0;
	virtual void OnEntityCreateMove(Entity* entity, NetworkPacket* packet) = 0;
	virtual void HandlePacket(NetworkPacket* packet) = 0;
	virtual void EntityStreamIn(Entity* ent) = 0;
	virtual void EntityStreamOut(Entity* ent) = 0;

	void EntityCM(Entity* ent);

	void NetworkCreateLagRecord(LagRecord& cmd);
	SAFE_PROP(std::queue<NetworkPacket*>, IncomingPackets);
	SAFE_PROP(std::queue<NetworkPacket*>, OutgoingPackets);

	ENetHost* Local = nullptr;
	ENetPeer* Peer = nullptr;

	SAFE_PROP(std::unordered_map <std::uint32_t COMMA Entity*>, Entities);

	std::mutex streamLock;
	std::vector<Entity*> RenderedEntities;
	std::vector<Entity*> StreamedEntities;
	std::atomic<bool> Connected{ false };
	std::atomic<bool> Running{ false };
	
	int NetworkTickrate = 30;

	int LastPacketId = 0;

	Grid<Entity*>* grid = nullptr;

	std::mutex gridLock;

	void ProcessStreamOut(NetworkPacket* packet);

	void ProcessStream(NetworkPacket* packet);

	static inline std::uint32_t CurrentFrameTime;



	void OnCreateMove();

	void OnFrameBegin();


	ClockTime connectedTime;

	std::condition_variable m_action_cond;
	std::mutex m_action_lock;
	std::queue<NetworkPacket*> m_actions;
};
