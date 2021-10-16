#pragma once
#include <winnt.h>
#include "natives.hpp"
#include "NativeInvoker.hpp"

template <typename Ret, typename ...Args>
FORCEINLINE Ret invoke(RAGE::ScrNativeHash hash, Args &&...args)
{
	

	NativeInvoker.BeginCall();
	(NativeInvoker.PushArg(std::forward<Args>(args)), ...);
	NativeInvoker.EndCall(hash);

	if constexpr (!std::is_same_v<Ret, void>)
	{
		return NativeInvoker.GetReturnValue<Ret>();
	}
}
#define NATIVE_DECL __declspec(noinline) inline

namespace GAMEPLAY {
	NATIVE_DECL int GET_GAME_TIMER() { return invoke<int>(0x9CD27B0045628463); } // 0x9CD27B0045628463 0xA4EA0691
}