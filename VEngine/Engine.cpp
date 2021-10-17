#include "pch.h"
#include "Engine.hpp"
#include "Utilities.hpp"
#include "Pointer.h"
#include "Functions.hpp"
#define fullHashMapDepth 24
int Engine::GameVersionToSearchDepth(int version)
{
	switch (version) {
	case 0:
	case 1:
		return 0;
	case 2:
	case 3:
		return 1;
	case 4:
	case 5:
		return 2;
	case 6:
	case 7:
	case 8:
	case 9:
		return 3;
	case 10:
	case 11:
		return 4;
	case 12:
	case 13:
		return 5;
	case 14:
	case 15:
		return 6;
	case 16:
	case 17:
		return 7;
	case 18:
	case 19:
		return 8;
	case 20:
	case 21:
	case 22:
	case 23:
		return 9;
	case 24:
	case 25:
		return 10;
	case 26:
	case 27:
		return 11;
	case 28:
	case 29:
		return 12;
	case 30:
	case 31:
	case 32:
	case 33:
		return 13;
	case 34:
	case 35:
		return 14;
	case 36:
	case 37:
		return 15;
	case 38:
	case 39:
		return 16;
	case 40:
	case 41:
		return 17;
	case 42:
	case 43:
	case 44:
	case 45:
		return 18;
	case 46:
	case 47:
	case 48:
	case 49:
		return 19;
	case 50:
	case 51:
	case 52:
	case 53:
		return 20;
	case 54:
	case 55:
	case 56:
	case 57:
	case 58:
		return 21;
	case 59:
	case 60:
	case 61:
	case 62:
	case 63:
		return 22;
	case 64:
	case 65:
	case 66:
	case 67:
	case 68:
		return 23;
	default:
		return fullHashMapDepth - 1;
	}
}

void Engine::InitalizePointers() {

	std::cout << "Initializing engine." << std::endl;

	{
		auto ptr = Memory::Pointer(MEMORY::PatternScan("48 8D 0D ? ? ? ? 48 8B 14 FA E8 ? ? ? ? 48 85 C0 75 0A"));
		Functions::m_native_registration_table = ptr.add(3).rip().as<RAGE::ScrNativeRegistrationTable*>();
		Functions::m_get_native_handler = ptr.add(12).rip().as<Functions::get_native_handler_t>();
	}
	{
		auto ptr = Memory::Pointer(MEMORY::PatternScan("83 79 18 00 48 8B D1 74 4A FF 4A 18 48 63 4A 18 48 8D 41 04 48 8B 4C CA"));
		Functions::m_fix_vectors = ptr.as<Functions::fix_vectors_t>();
	}
	{
		auto ptr = Memory::Pointer(MEMORY::PatternScan("45 33 F6 8B E9 85 C9 B8"));

		Functions::m_script_threads = ptr.sub(4).rip().sub(8).as<decltype(Functions::m_script_threads)>();
		Functions::m_run_script_threads = ptr.sub(0x1F).as<Functions::run_script_threads_t>();
	}
	{
		GameVersion = Functions::GetGameVersion();
		std::cout << "Game version " << GameVersion << std::endl;
	}


	{
		NativeInvoker.Initialize();

	}
}