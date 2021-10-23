#pragma once

enum  PacketFlags
{
	None = 0,
	StreamEntireNetwork = 1,
	StreamRangeNetwork = 2,
	StreamRangeEntities = 3,
};
enum PacketType
{
	PlayerHandshake = 0,
	PlayerJoin = 1,
	EntitiesStreamIn = 2,
	PlayerCreateMove = 3,
	EntitiesStreamOut = 4
};
class NetworkPacket {
public:

	~NetworkPacket();


#if SERVER
	/// <summary>
	/// Used in streaming data to multiple entities
	/// </summary>
	/// <param name="peer"></param>
	/// <param name="packet"></param>
	NetworkPacket(int packetType, std::vector<Entity*> batch, int flags = 0, ENetPeer* except = nullptr);

	/// <summary>
	/// Used in streaming data to only one entity
	/// </summary>
	/// <param name="peer"></param>
	/// <param name="packet"></param>
	NetworkPacket(int packetType, ENetPeer* peer);

	/// <summary>
	/// Used in cases where we need flags
	///	like reliable udp packet or compressed packet
	/// </summary>
	/// <param name="peer"></param>
	/// <param name="packet"></param>
	/// <param name="flags"></param>
	NetworkPacket(int packetType, ENetPeer* peer, int flags = 0);

	/// <summary>
	/// Used in case if we want to stream data over all entities
	///	Except -> nullptr in case we need to skip one player
	/// </summary>
	/// <param name="packet"></param>
	/// <param name="flags"></param>
	NetworkPacket(int packetType,  int flags, ENetPeer* except = nullptr);


	std::vector<Entity*> EntitiesBatch;



#endif

	void Send();

	/// <summary>
	/// Both exists in client and server
	///	Used for incomming packets on both sides.
	/// </summary>
	/// <param name="peer"></param>
	/// <param name="packet"></param>
	NetworkPacket(ENetPeer* peer, ENetPacket* packet);

	NetworkPacket(int packetType, int flags);

	uint8_t* mData;                         /**< Data building up. */
	unsigned int mPos;                   /**< Position in the data. */
	unsigned int mDataSize;              /**< Allocated datasize. */

	bool Reliable = false;
	bool Outgoing = false;
	bool Executed = false;

    int Flags = 0;
	int Type = -1;

	size_t Current = 0;
	size_t Lenght = 0;
	size_t LenghtAllocated = 0;

	ENetPacket* Packet = nullptr;
	ENetPeer* TargetPeer = nullptr;
	ENetPeer* ExceptPeer = nullptr;

	size_t ReadRaw(void* dst, size_t sz);
	void WriteRaw(const void* src, size_t sz);

	inline void MarkAsExecuted() {
		Executed = true;
	}


	template<typename T>
	inline size_t Read(T& dst)
	{
		size_t sz = ReadRaw(&dst, sizeof(T));
		assert(sz == sizeof(T));
		return sz;
	}

	template<typename T>
	inline void Write(const T& src)
	{
		WriteRaw(&src, sizeof(T));
	}

	inline void Write(const char* src)
	{
		uint32_t len = (uint32_t)strlen(src);
		Write(len);
		WriteRaw(src, len);
	}

	inline void Write(const std::string& src)
	{
		uint32_t len = (uint32_t)src.size();
		Write(len);
		WriteRaw(src.c_str(), src.size());
	}
};