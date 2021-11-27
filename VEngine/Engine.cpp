#include "pch.h"


#include "Utilities.hpp"
#include "Pointer.h"
#include "Functions.hpp"
#include "Engine.hpp"
#include "Multiplayer.hpp"
#include "NativeThreadsafeExecutor.hpp"
#include <VEngine\nativeList.hpp>

#include "Pattern.hpp"
static std::pair<RAGE::GtaThread*, int> org_thread;
static uint32_t* g_thread_id;
static uint32_t* g_thread_count;
RAGE::GtaThread* managedThread;

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
	} {
		PointerToHandle = (pointer_to_handle_t)MEMORY::PatternScan("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC 20 8B 15 ? ? ? ? 48 8B F9 48 83 C1 10 33 DB");

		HandleToPointer = (handle_to_pointer_t)MEMORY::PatternScan("83 f9 ff 74 31 4c 8b 0d ? ? ? ? 44 8b c1 49 8b 41 08");
	}
	{
		GameVersion = Functions::GetGameVersion();
		std::cout << "Game version " << GameVersion << std::endl;
	}
	{
		
		ReplayInterface = *Memory::as_relative<CReplayInterface**>(MEMORY::PatternScan("48 8D 0D ? ? ? ? 48 8B D7 E8 ? ? ? ? 48 8D 0D ? ? ? ? 8A D8 E8 ? ? ? ? 84 DB 75 13 48 8D 0D"));
	
	}
	{

		auto g_scriptHandlerMgrPattern = Memory::pattern("74 17 48 8B C8 E8 ? ? ? ? 48 8D 0D");

		auto location = g_scriptHandlerMgrPattern.count(1).get(0).get<char>(13);
		g_scriptHandlerMgr = reinterpret_cast<decltype(g_scriptHandlerMgr)>(location + *(int32_t*)location + 4);

		location = Memory::pattern("FF 40 5C 8B 15 ? ? ? ? 48 8B").count(1).get(0).get<char>(5);
		g_thread_id = reinterpret_cast<decltype(g_thread_id)>(location + *(int32_t*)location + 4);
		 printf("ScriptHook: g_thread_id %llx", g_thread_id);

		location -= 9;
		g_thread_count = reinterpret_cast<decltype(g_thread_count)>(location + *(int32_t*)location + 4);

		printf("ScriptHook: g_thread_count %llx", g_thread_count);
	}

	{
		NativeInvoker.Initialize();

	}
}


void Engine::MainScript()
{
	std::cout << "hello from main script " << std::endl;
	//while (true) {
		/*if (GetAsyncKeyState(0x4C))
		{
			STREAMING::REQUEST_MODEL(0x705E61F2);
			while (!STREAMING::HAS_MODEL_LOADED(0x705E61F2))
				Wait(0);

			std::cout << "Model has been loaded  " << std::endl;

		}*/


		NativeThreadSafe::ExecuteNativeQueue();

		Multiplayer.CurrentFrameTime = enet_time_get();

		Multiplayer.OnCreateMove();

		

	//	Wait(0);
//	}
}

void Engine::AttachScriptThread(RAGE::GtaThread* new_thread)
{
	// get a free thread slot
	auto collection = rage::script_thread_array;
	int slot = 0;

	for (auto& thread : *collection)
	{
		auto context = thread->GetContext();
		if (context->ThreadId == 0)
		{
			break;
		}

		slot++;
	}

	// did we get a slot?
	if (slot == collection->count())
	{
		return;
	}

	new_thread->reset((*g_thread_count) + 1, nullptr, 0);

	if (*g_thread_id == 0) {
		(*g_thread_id)++;
	}

	new_thread->m_context.m_thread_id = *g_thread_id;
	new_thread->m_handler = g_script_handler_manager;

	(*g_thread_id)++;
	(*g_thread_count)++;

	org_thread = std::pair<RAGE::GtaThread*, int>(collection->at(slot), slot);

	collection->set(slot, new_thread);

	printf("ScriptHook: Attached a new script thread with id: %d at %llx\n", new_thread->m_instance_id, new_thread);
}

bool EngineWorker::Initialize()
{
	return false;
}
