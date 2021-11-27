#pragma once

class Entity;

enum  PacketFlags
{
	None = 1 << 0,
	StreamEntireNetwork = 1 << 1,
	StreamRangeEntities = 1 << 2,
};
enum PacketType
{
	PlayerHandshake = 0,
	PlayerJoin = 1,
	EntitiesStreamIn = 2,
	PlayerCreateMove = 3,
	EntitiesStreamOut = 4,
	PlayerLeave = 5
};
class NetworkPacket {
public:


#if SERVER
	/// <summary>
/// Send packet to batch of entities
/// </summary>
/// <param name="packetType"></param>
/// <param name="batch"></param>
/// <param name="Packetflags"></param>
/// <param name="except"></param>
/// <param name="Reliable"></param>
	NetworkPacket(int packetType, std::vector<Entity*>& batch, int Packetflags = 0, ENetPeer* except = nullptr, bool Reliable = false);

	/// <summary>
	/// Send packet to one peer or entire network
	///	Warning! When peer is nullptr it streams entire network
	/// </summary>
	/// <param name="packetType"></param>
	/// <param name="peer"></param>
	/// <param name="Packetflags"></param>
	/// <param name="except"></param>
	/// <param name="Reliable"></param>
	NetworkPacket(int packetType, ENetPeer* peer = nullptr, bool Reliable = false, int Packetflags = 0, ENetPeer* except = nullptr);




	std::vector<Entity*>* EntitiesBatch = nullptr;



#endif

	void Send();

	/// <summary>
	/// Both exists in client and server
	///	Used for incomming packets on both sides.
	/// </summary>
	/// <param name="peer"></param>
	/// <param name="packet"></param>
	NetworkPacket(ENetPeer* peer, ENetPacket* packet);

	NetworkPacket(int packetType, bool Reliable = false);

	uint8_t* mData;                         /**< Data building up. */
	unsigned int mPos;                   /**< Position in the data. */
	unsigned int mDataSize;              /**< Allocated datasize. */

	bool Reliable = false;
	bool Outgoing = false;
	bool Executed = false;

    int PacketFlags = 0;
	int NetworkFlags = 0;

	int Type = -1;

	size_t Current = 0;
	size_t Lenght = 0;
	size_t LenghtAllocated = 0;

	void Reset()
	{
		Current = 0;
	}

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

	~NetworkPacket();

};