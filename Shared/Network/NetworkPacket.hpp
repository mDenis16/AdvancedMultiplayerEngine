#pragma once

class NetworkPacket {
public:
	NetworkPacket(ENetPeer* peer, ENetPacket* packet);
	
	uint8_t* mData;                         /**< Data building up. */
	unsigned int mPos;                   /**< Position in the data. */
	unsigned int mDataSize;              /**< Allocated datasize. */

	bool Reliable = false;
	bool Outgoing = false;
	bool Executed = false;

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