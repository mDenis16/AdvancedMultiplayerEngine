#pragma once
#include "Script.hpp"
#include <VEngine\Game.hpp>

#include "gtaThread.h"


class EngineWorker
{
public:

	bool Initialize();

};

namespace Engine {

	inline CReplayInterface* ReplayInterface = nullptr;
	inline bool FailedInitialize = false;

	//int GameVersionToSearchDepth(int version);

	//void InitalizePointers();

	//void Wait(std::optional<std::chrono::high_resolution_clock::duration> time);

	//void MainScript();

	//using pointer_to_handle_t = int32_t(*)(void* ptr);
	//using handle_to_pointer_t = int64_t(*)(int handle);

	//inline RAGE::scriptHandlerMgr* g_scriptHandlerMgr;
	//
	//inline handle_to_pointer_t HandleToPointer = nullptr;
	//inline pointer_to_handle_t PointerToHandle = nullptr;
	//inline int GameVersion = 0;
	//inline int SearchDept = 0;

	//inline void* m_main_fiber = nullptr;

	//inline void* m_script_fiber = nullptr;

	//inline std::optional<std::chrono::high_resolution_clock::time_point> m_wake_time;
	//void AttachScriptThread(RAGE::GtaThread* new_thread);

	//
	//inline script mainScriptt(MainScript);
	//inline void Wait(int ms )
	//{
	//	mainScriptt.yield(std::chrono::milliseconds(ms));
	//}
}
