#include "pch.h"
#include "Hooks.hpp"
#include <MinHook.h>
#include "EventsHooks.hpp"
#include "Utilities.hpp"
#include "Pointer.h"
#include "Engine.hpp"
#include "ScriptHook.hpp"
#include "Functions.hpp"
#include "nativeList.hpp"

namespace Hooks {

	void* run_script_threads_original = nullptr;
	//using GetEventData = bool(std::int32_t eventGroup, std::int32_t eventIndex, std::int64_t* args, std::uint32_t argCount);
	//GetEventData* m_GetEventData = nullptr;


	//void* m_OriginalGetEventData = nullptr;


	//static bool GetEventDataHook(std::int32_t eventGroup, std::int32_t eventIndex, std::int64_t* args, std::uint32_t argCount) {
	//	auto ret = static_cast<decltype(&GetEventDataHook)>(m_OriginalGetEventData)(eventGroup, eventIndex, args, argCount);
	static  int numFrames = 0;
	static int startTime = -1;
	bool run_script_threads(std::uint32_t ops_to_execute)
	{
		if (Engine::FailedInitialize) return false;

		TRY_CLAUSE
		{
			bool skip = false;
		if (startTime == -1) {
			startTime = GAMEPLAY::GET_GAME_TIMER();
			skip = true;
		}
		
			++numFrames;
			auto elapsedMS = GAMEPLAY::GET_GAME_TIMER() - startTime;
			if (!skip) {
				double elapsedSeconds = elapsedMS / 1000.0; // Convert to seconds
				double fps = numFrames / elapsedSeconds; // FPS is Frames / Seconds
				std::cout << "fps : " << fps << std::endl;
			}
		
			 auto ret = static_cast<decltype(&run_script_threads)>(run_script_threads_original)(ops_to_execute);

			 
			
			 return ret;
		} EXCEPT_CLAUSE
			return false;
	}


	void InitializeRunScriptHook() {
		std::cout << "Initializing RunScriptHook" << std::endl;

		{

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