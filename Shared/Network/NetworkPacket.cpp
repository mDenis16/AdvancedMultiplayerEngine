
#include <Common.hpp>
#include <Shared.hpp>
#include <enet\enet.h>
#include "NetworkPacket.hpp"
#if SERVER
#include "../Server/Network.h"
#endif
#if CLIENT
#include "../Client/Network.hpp"
#include "../EngineSimulator/Multiplayer.hpp"
#endif


#define NETWORK_MESSAGE_DEFAULT_SIZE 32
#define NETWORK_MESSAGE_ALLOCATE_SIZE 64
#undef min

#if SERVER
NetworkPacket::NetworkPacket(int packetType, std::vector<Entity*> batch, int flags , ENetPeer* except)
{
	//Reliable = (packet->flags & ENET_PACKET_FLAG_RELIABLE) != 0;
	Flags = flags;
	Flags |= StreamRangeEntities;
	Outgoing = true;
	Type = packetType;
	EntitiesBatch = batch;
	ExceptPeer = except;
	TargetPeer = nullptr;
	Write(Type);
}

NetworkPacket::NetworkPacket(int packetType, int flags, ENetPeer* except)
{
	//Reliable = (packet->flags & ENET_PACKET_FLAG_RELIABLE) != 0;
	Flags = flags;
	Outgoing = true;
	Type = packetType;
	ExceptPeer = except;
	TargetPeer = nullptr;
	Write(Type);
}

NetworkPacket::NetworkPacket(int packetType, ENetPeer* peer, int flags)
{
	Flags = flags;
	flags |= ENET_PACKET_FLAG_RELIABLE;
	Outgoing = true;
	Type = packetType;

	TargetPeer = peer;
	Write(Type);
}
#endif

NetworkPacket::NetworkPacket(ENetPeer* peer, ENetPacket* packet)
{
	Reliable = (packet->flags & ENET_PACKET_FLAG_RELIABLE) != 0;

	TargetPeer = peer;
	if (packet->data)
	   mData = packet->data;
	Lenght = packet->dataLength;
	Packet = packet;
	if (packet->data)
	   Read(Type);

}

NetworkPacket::NetworkPacket(int packetType, int flags)
{
	Flags = flags;
	Outgoing = true;
	Type = packetType;
	Write(Type);
}


void NetworkPacket::Send()
{
#if SERVER
	{
		{
			std::lock_guard<std::mutex> guard(GameNetwork.OutgoingPackets_lock);
			GameNetwork.OutgoingPackets.push(this);
		}
		{std::lock_guard<std::mutex> guard2(GameNetwork.HandleOutgoingFlowMutex);
		GameNetwork.HandleOutgoingFlowMutexCondFlag = true;
		GameNetwork.HandleOutgoingFlowMutexCond.notify_all();
		}
		std::cout << "Added packet outgoing to queue" << std::endl;
	}
	
#endif

#if CLIENT
	{
		std::lock_guard<std::mutex> guard(Multiplayer.OutgoingPackets_lock);
		Multiplayer.OutgoingPackets.push(this);
	}
	{
		std::lock_guard<std::mutex> guard2(Multiplayer.HandleOutgoingFlowMutex);
     	Multiplayer.HandleOutgoingFlowMutexCondFlag = true;
	    Multiplayer.HandleOutgoingFlowMutexCond.notify_all();

	}
#endif

	Flags |= ENET_PACKET_FLAG_NO_ALLOCATE;
}


NetworkPacket::~NetworkPacket()
{
	if (Packet != nullptr) {
		enet_packet_destroy(Packet);
	}
	else if (mData != nullptr) {
		free(mData);
	}
}
size_t NetworkPacket::ReadRaw(void* dst, size_t sz)
{
	assert(!Outgoing);
	assert(Current < Lenght);


	sz = std::min(sz, Lenght - Current);


	assert(sz > 0);
	if (sz == 0) {
		return 0;
	}

	memcpy(dst, mData + Current, sz);
	Current += sz;

	return sz;
}

void NetworkPacket::WriteRaw(const void* src, size_t sz)
{

	assert(Outgoing);

	if (mData == nullptr) {
		LenghtAllocated = NETWORK_MESSAGE_DEFAULT_SIZE;
		mData = (uint8_t*)malloc(LenghtAllocated);
	}

	bool mustRealloc = false;
	while (Current + sz > LenghtAllocated) {
		LenghtAllocated += NETWORK_MESSAGE_ALLOCATE_SIZE;
		mustRealloc = true;
	}

	if (mustRealloc) {
		mData = (uint8_t*)realloc(mData, LenghtAllocated);
	}

	memcpy(mData + Current, src, sz);
	Lenght += sz;
	Current += sz;

}
