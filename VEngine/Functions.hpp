#pragma once
#include "natives.hpp"
#include "NativeInvoker.hpp"
#include "script_program.hpp"

namespace Functions {

    int GetGameState();

    using get_native_handler_t = RAGE::ScrNativeHandler(*)(RAGE::ScrNativeRegistrationTable*, RAGE::ScrNativeHash);
    inline get_native_handler_t m_get_native_handler{};

    inline RAGE::ScrNativeRegistrationTable* m_native_registration_table{};

    inline RAGE::scrProgramTable* m_script_program_table{};
    int GetGameVersion();

    using fix_vectors_t = void(*)(RAGE::ScrNativeCallContext*);
    inline fix_vectors_t m_fix_vectors{};
}