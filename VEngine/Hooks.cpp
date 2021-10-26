#include "pch.h"
#include "Hooks.hpp"
#include <MinHook.h>
#include "EventsHooks.hpp"
#include "Utilities.hpp"
#include "Pointer.h"
#include "Engine.hpp"
#include "ScriptHook.hpp"
#include "Functions.hpp"
#include "Multiplayer.hpp"
#include "nativeList.hpp"
#include "NativeThreadsafeExecutor.hpp"
#include <VEngine\Script.hpp>

namespace Hooks {

	void* run_script_threads_original = nullptr;
	void* is_dlc_present_original = nullptr;
	void* convert_thread_to_fiber_original = nullptr;
	std::optional<std::chrono::high_resolution_clock::time_point> m_wake_time;
;
	void* m_main_fiber;
	void* m_script_fiber;


	static  int numFrames = 0;
	static int startTime = -1;
	void Wait(int ms);


;

	bool run_script_threads(std::uint32_t ops_to_execute)
	{
		if (Engine::FailedInitialize) return false;

		auto ret = static_cast<decltype(&run_script_threads)>(run_script_threads_original)(ops_to_execute);

	
			
		return ret;
		
	}


	bool is_dlc_present(std::uint32_t hash)
	{
		
		

		static bool ensure_main_fiber = (ConvertThreadToFiber(nullptr), true);
		static bool notInitialized = true;

		Engine::mainScriptt.tick();


		return static_cast<decltype(&is_dlc_present)>(is_dlc_present_original)(hash);
	}
	void* convert_thread_to_fiber(void* param)
	{
		TRY_CLAUSE
		{
			if (IsThreadAFiber())
			{
				return GetCurrentFiber();
			}

			return static_cast<decltype(&convert_thread_to_fiber)>(convert_thread_to_fiber_original)(param);
		
		} EXCEPT_CLAUSE
			return nullptr;
	}

	void InitializeRunScriptHook() {
		std::cout << "Initializing RunScriptHook" << std::endl;

		{

			auto address = MEMORY::PatternScan("48 89 5C 24 ? 57 48 83 EC 20 81 F9");
			auto converthreadAddress = (void*)GetProcAddress(GetModuleHandleA("kernel32.dll"), "ConvertThreadToFiber"); // .as<void*>();
			
			if (MH_CreateHook(converthreadAddress, &convert_thread_to_fiber, reinterpret_cast<void**>(&convert_thread_to_fiber_original)) != MH_OK) {
				Engine::FailedInitialize = true;
				std::cout << "Failed hooking convert_thread_to_fiber " << std::endl;
				return;
			}


			if (MH_CreateHook(address, &is_dlc_present, reinterpret_cast<void**>(&is_dlc_present_original)) != MH_OK) {
				Engine::FailedInitialize = true;
				std::cout << "Failed hooking run is dlc present " << std::endl;
				return;
			}

			if (MH_CreateHook(Functions::m_run_script_threads, &run_script_threads, reinterpret_cast<void**>(&run_script_threads_original)) != MH_OK) {
				Engine::FailedInitialize = true;
				std::cout << "Failed hooking run script threads " << std::endl;
				return;
			}
			std::cout << "Initialized RunScriptHook successfully!" << std::endl;
		}
	}
	void Initialize() {
		MH_Initialize();
		InitializeRunScriptHook();
		EventsHooks::Initialize();

	}
}