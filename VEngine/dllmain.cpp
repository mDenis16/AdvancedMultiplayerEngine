// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "Functions.hpp"
#include "Hooks.hpp"
#include "Engine.hpp"
#include <MinHook.h>
#include <chrono>
#include <VEngine\NativeThreadsafeExecutor.hpp>

#include "Multiplayer.hpp"
#include "nativeList.hpp"
#include "GameRender.hpp"


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

 
   //Multiplayer.Initialize();
   //GameRender::Initialize();

    while (!Engine::FailedInitialize) {

        if (GetAsyncKeyState(0x4C))
        {
            NATIVE_FUNCTION({
    auto localPed = PLAYER::GET_PLAYER_PED(0);
    if (localPed)
        ENTITY::SET_ENTITY_COORDS_NO_OFFSET(localPed, -763.4022, 7.327758, 40.59016, true, true, true);

                }, 0);


            NATIVE_END()
        }
        if (GetAsyncKeyState(0x4B)) {

     
          
            /*NativeThreadSafe::QueueJob* Job = new NativeThreadSafe::QueueJob();
            Job->Callbacks.resize(2);
            Job->Execute([Job,]
                {
                    auto model = GAMEPLAY::GET_HASH_KEY("mp_m_freemode_01");

                    while (!STREAMING::HAS_MODEL_LOADED(model))
                        SYSTEM::WAIT(0);

                    auto ret = PED::CREATE_PED(26, model, -763.4022, 7.327758, 40.59016, 0, false, true);
                    Job->Callbacks[0] = ret;

                    ENTITY::SET_ENTITY_INVINCIBLE(ret, true);
                    ENTITY::FREEZE_ENTITY_POSITION(ret, true);

                    PED::SET_PED_CAN_BE_TARGETTED(ret, true);
                    PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(Job->Callbacks[0], true);

                    PED::SET_PED_RELATIONSHIP_GROUP_HASH(ret, 0);
                    PED::SET_PED_CAN_RAGDOLL(ret, false);
                    PED::SET_PED_DEFAULT_COMPONENT_VARIATION(ret);
                    PED::SET_PED_CAN_EVASIVE_DIVE(ret, false);
                    PED::SET_PED_GET_OUT_UPSIDE_DOWN_VEHICLE(ret, false);
                    PED::SET_PED_AS_ENEMY(ret, false);
                    PED::SET_CAN_ATTACK_FRIENDLY(ret, true, false);
                    PED::SET_PED_ALLOW_VEHICLES_OVERRIDE(ret, true);
                    PED::SET_PED_CONFIG_FLAG(ret, 281, true);

                    std::cout << "spawned ped " <<  std::endl;
                });
            Job->WaitforCallback();
            std::cout << "Created ped with handle " << Job->Callbacks[0] << std::endl;*/

        }

        if (GetAsyncKeyState(0x2E)) {
            Engine::FailedInitialize = true;
        }
        Sleep(100);
    }

    Sleep(500);

    std::cout << "Failed initialize. Closing" << std::endl;

    Multiplayer.Destroy();
    GameRender::Destruct();

    std::cout << "Destroying multiplayer " << std::endl;
    Sleep(1000);
    fclose((_iobuf*)__acrt_iob_func(0));
    fclose((_iobuf*)__acrt_iob_func(1));
    fclose((_iobuf*)__acrt_iob_func(2));

    FreeConsole();

    MH_DisableHook(MH_ALL_HOOKS);
    MH_Uninitialize();

    Sleep(5000);
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

