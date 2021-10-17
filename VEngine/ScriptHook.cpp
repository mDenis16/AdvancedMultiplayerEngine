#include "pch.h"
#include "VmtHook.hpp"
#include "ScriptHook.hpp"
#include "crossmap.hpp"
#include "script_program.hpp"
#include "natives.hpp"
#include "Functions.hpp"

namespace RAGE {
;

	void script_exception_handler(PEXCEPTION_POINTERS exp)
	{
		HMODULE mod{};
		DWORD64 offset{};
		char buffer[MAX_PATH]{};
		if (GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCWSTR)exp->ExceptionRecord->ExceptionAddress, &mod) == TRUE && mod != nullptr)
		{
			offset = ((DWORD64)exp->ExceptionRecord->ExceptionAddress - (DWORD64)mod);
			GetModuleFileNameA(mod, buffer, MAX_PATH - 1);
		}
		std::cout << "Exception Code: " << HEX_TO_UPPER(exp->ExceptionRecord->ExceptionCode) << " Exception Offset: " << HEX_TO_UPPER(offset) << " Fault Module Name: " << buffer;
	}



}
