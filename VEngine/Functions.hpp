#pragma once
#include "natives.hpp"
#include "NativeInvoker.hpp"
#include "script_program.hpp"
#include "jooat.hpp"
#include "array.hpp"
#include "gtaThread.h"

namespace Functions {

    int GetGameState();

    using get_native_handler_t = RAGE::ScrNativeHandler(*)(RAGE::ScrNativeRegistrationTable*, RAGE::ScrNativeHash);
    inline get_native_handler_t m_get_native_handler{};

    inline RAGE::ScrNativeRegistrationTable* m_native_registration_table{};

    inline RAGE::scrProgramTable* m_script_program_table{};
    int GetGameVersion();

    using fix_vectors_t = void(*)(RAGE::ScrNativeCallContext*);
    inline fix_vectors_t m_fix_vectors{};

    using run_script_threads_t = bool(*)(std::uint32_t ops_to_execute);
    inline run_script_threads_t m_run_script_threads{};

    inline RAGE::atArray<RAGE::GtaThread*>* m_script_threads{};


    template <typename F, typename ...Args>
    void execute_as_script(RAGE::joaat_t script_hash, F&& callback, Args &&...args);

}