#pragma once
#include "Script.hpp"


namespace Engine {

	inline bool FailedInitialize = false;

	int GameVersionToSearchDepth(int version);

	void InitalizePointers();

	void Wait(std::optional<std::chrono::high_resolution_clock::duration> time);

	void MainScript();

	inline int GameVersion = 0;
	inline int SearchDept = 0;

	inline void* m_main_fiber = nullptr;

	inline void* m_script_fiber = nullptr;

	inline std::optional<std::chrono::high_resolution_clock::time_point> m_wake_time;

	inline script mainScriptt(MainScript);
	inline void Wait(int ms )
	{
		mainScriptt.yield(std::chrono::milliseconds(ms));
	}
}
