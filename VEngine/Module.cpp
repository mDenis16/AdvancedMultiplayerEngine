#include "pch.h"
#include "Pattern.hpp"
#include "Range.hpp"
#include "Handle.hpp"
#include "Module.hpp"

namespace Memory
{
	Module::Module(HMODULE mod) :
		Range(mod, 0)
	{
		auto dosHeader = Base.As<IMAGE_DOS_HEADER*>();
		auto ntHeader = Base.Add(dosHeader->e_lfanew).As<IMAGE_NT_HEADERS*>();
		mSize = ntHeader->OptionalHeader.SizeOfImage;
	}

	Module::Module(std::nullptr_t) :
		Module(GetModuleHandle(nullptr))
	{
	}

	Module::Module(std::string_view name) :
		Module(GetModuleHandleA(name.data()))
	{
	}

	Module::Module(std::wstring_view name) :
		Module(GetModuleHandleW(name.data()))
	{
	}

	Handle Module::GetExport(std::string_view symbol_name)
	{
		return GetProcAddress(Base.As<HMODULE>(), symbol_name.data());
	}
}
