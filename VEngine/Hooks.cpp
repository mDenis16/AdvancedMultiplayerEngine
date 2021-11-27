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

#include "Pattern.hpp"

namespace Hooks {

	void* run_script_threads_original = nullptr;
	void* is_dlc_present_original = nullptr;
	void* convert_thread_to_fiber_original = nullptr;
	std::optional<std::chrono::high_resolution_clock::time_point> m_wake_time;
;
	void* m_main_fiber;
	void* m_script_fiber;
	void* m_on_game_frame;


	void* on_game_frame_original = nullptr;

	static  int numFrames = 0;
	static int startTime = -1;
	void Wait(int ms);


	void on_game_frame_hook()
	{
		std::cout << "Hello from game frame " << std::endl;

	}
;

	bool run_script_threads(std::uint32_t ops_to_execute)
	{
		//if (Engine::FailedInitialize) return false;

		TRY_CLAUSE
		{

			static bool Init = false;

		if (GetAsyncKeyState(0x45)) {
			SCRIPT::_BEGIN_ENUMERATING_THREADS();
			int id = SCRIPT::_GET_ID_OF_NEXT_THREAD_IN_ENUMERATION();

			while (id != 0) {
				id = SCRIPT::_GET_ID_OF_NEXT_THREAD_IN_ENUMERATION();
				if (id != 0) {
					SCRIPT::TERMINATE_THREAD(id);
					std::cout << "Terminated thread  id "  <<  id <<  std::endl;
				}
				std::cout << "Found script running under name " << SCRIPT::_GET_NAME_OF_THREAD(id) << std::endl;
			}
		}


	      	return static_cast<decltype(&run_script_threads)>(run_script_threads_original)(ops_to_execute);
		} EXCEPT_CLAUSE
		return static_cast<decltype(&run_script_threads)>(run_script_threads_original)(ops_to_execute);
	}


	bool is_dlc_present(std::uint32_t hash)
	{


		static bool ensure_main_fiber = (ConvertThreadToFiber(nullptr), true);
		static bool notInitialized = true;
		static uint64_t	last = 0;
		uint64_t		cur = GAMEPLAY::GET_FRAME_COUNT();
		/*if (last != cur)
		{
			last = cur;*/
			Engine::mainScriptt.tick();
		//}
	

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

			auto ptr = MEMORY::PatternScan("83 3F 00 76 ? FF D5 39");

			auto addr = Memory::pattern("85 F6 78 ? 75 ? 83 FB 01 75").count(1).get(0).get<char>(-0x2A);


		
			if (MH_CreateHook(addr, &on_game_frame_hook, reinterpret_cast<void**>(&on_game_frame_original)) != MH_OK) {
				Engine::FailedInitialize = true;
				std::cout << "Failed hooking on_game_frame " << std::endl;
				MessageBoxA(nullptr, "Failed hooking on_game_frame ", "Error", MB_OK | MB_ICONQUESTION);
				return;
			}


			/*if (MH_CreateHook(address, &is_dlc_present, reinterpret_cast<void**>(&is_dlc_present_original)) != MH_OK) {
				Engine::FailedInitialize = true;
				std::cout << "Failed hooking run is dlc present " << std::endl;
				return;
			}*/

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
