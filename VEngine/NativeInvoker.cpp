#include "pch.h"
#include "natives.hpp"
#include "NativeInvoker.hpp"
#include "Functions.hpp"
#include "Utilities.hpp"
#include "Pointer.h"
#include "Pattern.hpp"
#include "Engine.hpp"

NativeCallContext::NativeCallContext()
{
	ReturnVal = &ReturnStack[0];
	Args = &ArgsStack[0];
}



RAGE::ScrNativeHandler CNativeInvoker::GetNativeHandler(uint64_t oldHash) {

#if _DEBUG && 0
	static int dumped_native_count = DumpAllNativeHashAndHandlers();
#endif

	auto cachePair = foundHashCache.find(oldHash);
	if (cachePair != foundHashCache.end()) {
		return cachePair->second;
	}

	RAGE::ScrNativeHandler handler = nullptr;
	uint64_t newHash = GetNewHashFromOldHash(oldHash);

	if (newHash == 0) {
		printf("Failed to GetNewHashFromOldHash(%llX)", oldHash);
		handler = nullptr;
	}
	else {
		NativeRegistration* table = registrationTable[newHash & 0xFF];
		for (; table; table = table->getNextRegistration()) {
			bool found = false;
			for (uint32_t i = 0; i < table->getNumEntries(); i++) {
				if (newHash == table->getHash(i)) {
					handler = table->handlers[i];
					found = true;
					break;
				}
			}
			if (found) break;
		}
	}
	foundHashCache[oldHash] = handler;
	return handler;
}

uint64_t CNativeInvoker::GetNewHashFromOldHash(uint64_t oldHash) {

	if (SearchDepth == 0) {
		std::cout << "SearchDepth  is 0 " << std::endl;
		// no need for conversion
		return oldHash;
	}

	
	for (int i = 0; i < fullHashMapCount; i++) {
		for (int j = 0; j <= SearchDepth; j++) {
			if (fullHashMap[i][j] == oldHash) {
				// found
				for (int k = SearchDepth; k > j; k--) {		
					uint64_t newHash = fullHashMap[i][k];
					if (newHash == 0)
						continue;
					return newHash;
				}
				
				return oldHash;
			}
		}
	}
	return 0;
}


void CNativeInvoker::BeginCall()
{
	CallContext.Reset();
}

void CNativeInvoker::EndCall(RAGE::ScrNativeHash hash)
{
	if (auto it = HandlerCache.find(hash); it != HandlerCache.end())
	{
		RAGE::ScrNativeHandler handler = it->second;

		handler(&CallContext);
		Functions::m_fix_vectors(&CallContext);
	}
	else
	{
		//[hash]() { log(WARNING) << "Failed to find " << HEX_TO_UPPER(hash) << " native's handler."; }();
	}
}

void CNativeInvoker::Initialize()
{
	auto ptr = Memory::Pointer(MEMORY::PatternScan("48 8D 0D ? ? ? ? 48 8B 14 FA E8 ? ? ? ? 48 85 C0 75 0A"));

	auto m_native_registration_table = ptr.add(3).rip().as<RAGE::ScrNativeRegistrationTable*>();
	auto m_get_native_handler = ptr.add(12).rip().as<Functions::get_native_handler_t>();

	SearchDepth = Engine::GameVersionToSearchDepth(Functions::GetGameVersion());

	Memory::executable_meta executable;
	executable.EnsureInit();

	auto registrationTablePattern = Memory::pattern("76 32 48 8B 53 40");

	auto location = registrationTablePattern.count(1).get(0).get<char>(9);
	if (location == nullptr) {

		printf("Unable to find registrationTable");
		return ;
	}

	registrationTable = reinterpret_cast<decltype(registrationTable)>(location + *(int32_t*)location + 4);
	printf("registrationTable\t 0x%p (0x%.8X)", registrationTable, reinterpret_cast<uintptr_t>(registrationTable) - executable.begin());

	unsigned int nativeCount = 0;
	for (const RAGE::ScrNativeMapping& mapping : RAGE::CrossMapList)
	{
		RAGE::ScrNativeHandler handler = m_get_native_handler(m_native_registration_table, mapping.second);

		

		if (handler) {
			nativeCount++;
			HandlerCache.emplace(mapping.first, handler);
			std::cout << "Native " << mapping.first << " handler " << handler << std::endl;
		}
	}

	std::cout << "Found  " << nativeCount << " good natives!" << std::endl;

}

