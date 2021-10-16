#include "pch.h"
#include "EventsHooks.hpp"
#include <MinHook.h>
#include "Utilities.hpp"
#include "Pointer.h"
#include "Engine.hpp"
#include "nativeList.hpp"


namespace EventsHooks {
	class fwEvent
	{
	public:
		virtual ~fwEvent() = 0;

		virtual void m_8() = 0;

		virtual bool Equals(fwEvent* other) = 0;

		virtual int32_t GetId() = 0;

		virtual int m_20() = 0;

		virtual int m_28() = 0;

		virtual bool GetArguments(void* buffer, size_t len) = 0;

		virtual bool m_38() = 0;

		virtual bool m_40(fwEvent* other) = 0;
	};

	using GetEventData = bool(std::int32_t eventGroup, std::int32_t eventIndex, std::int64_t* args, std::uint32_t argCount);
	GetEventData* m_GetEventData = nullptr;


	void* m_OriginalGetEventData = nullptr;


	static bool GetEventDataHook(std::int32_t eventGroup, std::int32_t eventIndex, std::int64_t* args, std::uint32_t argCount) {
		auto ret  = static_cast<decltype(&GetEventDataHook)>(m_OriginalGetEventData)(eventGroup, eventIndex, args, argCount);

		
		int evT = invoke<int>(0xD8F66A3A60C62153, eventGroup, eventIndex);

		std::cout << "evT " << evT << std::endl;

		
		return ret;
	}


	void Initialize() {

		

		std::cout << "Initializing Event Hooks " << std::endl;
		
		 auto addr = Memory::Pointer(MEMORY::PatternScan("48 85 C0 74 14 4C 8B 10"));

	

		 m_GetEventData = Memory::Pointer(addr).sub(28).as<decltype(m_GetEventData)>();

		if (MH_CreateHook(m_GetEventData, &GetEventDataHook, reinterpret_cast<void**>(&m_OriginalGetEventData)) != MH_OK) {
			throw std::runtime_error("failed enable GetEventData");
			Engine::FailedInitialize = true;
		}
		std::cout << "Hooked GetEventData  succesfully. " << std::endl;

		if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK) {
			throw std::runtime_error("failed to enable hooks.");
			Engine::FailedInitialize = true;
		}

		std::cout << "Enabled hooks succesfully. " << std::endl;
	}
}