// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "Functions.hpp"
#include "Hooks.hpp"
#include "Engine.hpp"
#include <MinHook.h>
#include <chrono>
#include <VEngine\NativeThreadsafeExecutor.hpp>
#include "nativeList.hpp"


unsigned long WINAPI initialize(void* instance) {
    AllocConsole();

    FILE* file = nullptr;
    freopen_s(&file, "conin$", "r", stdin);
    freopen_s(&file, "conout$", "w", stdout);
    freopen_s(&file, "conout$", "w", stderr);

    SetConsoleTitleA("VEngine Console");

  //  while (!FindWindow(L"grcWindow", L"Grand Theft Auto V"))
    //    std::this_thread::sleep_for(std::chrono::seconds(1));

    Engine::InitalizePointers();
    std::cout << "Initalizing hooks." << std::endl;


    Hooks::Initialize();

 


    while (!Engine::FailedInitialize) {

        if (GetAsyncKeyState(0x4B)) {
            NATIVE_FUNCTION({
    Job->Callbacks[0] = GAMEPLAY::GET_GAME_TIMER();
    Job->Callbacks[1] = 99;
                }, 2);

            std::cout << "GET_GAME_TIMER " << Job->Callbacks[0] << std::endl;

            NATIVE_END();
        }
        if (GetAsyncKeyState(0x2E)) {
            Engine::FailedInitialize = true;
        }
        Sleep(100);
    }

    Sleep(500);

    std::cout << "Failed initialize. Closing" << std::endl;

    fclose((_iobuf*)__acrt_iob_func(0));
    fclose((_iobuf*)__acrt_iob_func(1));
    fclose((_iobuf*)__acrt_iob_func(2));

    FreeConsole();

    MH_DisableHook(MH_ALL_HOOKS);
    MH_Uninitialize();
  
    FreeLibraryAndExitThread((HMODULE)instance, 0);
}
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        if (auto handle = CreateThread(nullptr, NULL, initialize, hModule, NULL, nullptr))
            CloseHandle(handle);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

