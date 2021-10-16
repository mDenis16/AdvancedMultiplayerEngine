#pragma once


namespace Engine {

	inline bool FailedInitialize = false;

	int GameVersionToSearchDepth(int version);

	void InitalizePointers();

	inline int GameVersion = 0;
	inline int SearchDept = 0;
}