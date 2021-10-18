#pragma once
#include <stack>


#define fullHashMapCount 6354
#define fullHashMapDepth 24

#pragma pack(push)
#pragma pack(4)		// _unknown 4 bytes
// https://www.unknowncheats.me/forum/grand-theft-auto-v/144028-reversal-thread-81.html#post1931323
struct NativeRegistration {
	uint64_t nextRegBase;
	uint64_t nextRegKey;
	RAGE::ScrNativeHandler handlers[7];
	uint32_t numEntries1;
	uint32_t numEntries2;
	uint32_t _unknown;
	uint64_t hashes[7];

	/*
		// decryption
		key = this ^ nextRegKey  // only lower 32 bits
		nextReg = nextRegBase ^ key<<32 ^ key
		// encryption
		key = this ^ nextRegKey  // only lower 32 bits
		nextRegBase = nextReg ^ key<<32 ^ key

		only lower 32 bits of this^nextRegKey are used, higher 32 bits are ignored.
		thus, higher 32 bit of nexRegBase must contain the info of (masked) higher address of next registration.
		the first two members of struct are named as Base/Key respectively in that sense.
	*/
	inline NativeRegistration* getNextRegistration() {
		uint32_t key = static_cast<uint32_t>(reinterpret_cast<uint64_t>(this) ^ nextRegKey);
		return reinterpret_cast<NativeRegistration*>(nextRegBase ^ (static_cast<uint64_t>(key) << 32) ^ key);
	}

	inline void setNextRegistration(NativeRegistration* nextReg, uint64_t nextKey) {
		nextRegKey = nextKey;
		uint32_t key = static_cast<uint32_t>(reinterpret_cast<uint64_t>(this) ^ nextRegKey);
		nextRegBase = reinterpret_cast<uint64_t>(nextReg) ^ (static_cast<uint64_t>(key) << 32) ^ key;
	}

	inline uint32_t getNumEntries() {
		return static_cast<uint32_t>(reinterpret_cast<uint64_t>(&numEntries1)) ^ numEntries1 ^ numEntries2;
	}

	inline uint64_t getHash(uint32_t index) {
		uint32_t key = static_cast<uint32_t>(reinterpret_cast<uint64_t>(&hashes[2 * index]) ^ hashes[2 * index + 1]);
		return hashes[2 * index] ^ (static_cast<uint64_t>(key) << 32) ^ key;
	}
};
#pragma pack(pop)

class NativeCallContext : public RAGE::ScrNativeCallContext
{
public:
	NativeCallContext();
private:
	std::uint64_t ReturnStack[10];
	std::uint64_t ArgsStack[100];
};

class CNativeInvoker
{
public:

	explicit CNativeInvoker() = default;
	~CNativeInvoker() = default;

	RAGE::ScrNativeHandler GetNativeHandler(uint64_t oldHash);


	uint64_t GetNewHashFromOldHash(uint64_t oldHash);

	

	void BeginCall();
	void EndCall(RAGE::ScrNativeHash hash);

	template <typename T>
	void PushArg(T&& value)
	{
		CallContext.PushArg(std::forward<T>(value));
	}

	template <typename T>
	T& GetReturnValue()
	{
		return *CallContext.GetReturnValue<T>();
	}

	uint64_t fullHashMap[fullHashMapCount][fullHashMapDepth];
	int SearchDepth = 0;

	void Initialize();
	NativeRegistration** registrationTable;
	std::unordered_map<uint64_t, RAGE::ScrNativeHandler> foundHashCache;
	
	

private:
	NativeCallContext CallContext;
	std::unordered_map<RAGE::ScrNativeHash, RAGE::ScrNativeHandler> HandlerCache;
};

inline CNativeInvoker NativeInvoker;

