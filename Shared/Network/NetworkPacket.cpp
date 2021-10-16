#include <Common.hpp>
#include <Shared.hpp>
#include <enet\enet.h>
#include "NetworkPacket.hpp"


#define NETWORK_MESSAGE_DEFAULT_SIZE 32
#define NETWORK_MESSAGE_ALLOCATE_SIZE 64
#undef min

NetworkPacket::NetworkPacket(ENetPeer* peer, ENetPacket* packet)
{
	Reliable = (packet->flags & ENET_PACKET_FLAG_RELIABLE) != 0;


	*mData = *packet->data;
	Lenght = packet->dataLength;
	Packet = packet;

	TargetPeer = peer;
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
