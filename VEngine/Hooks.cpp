#include "pch.h"
#include "Hooks.hpp"
#include <MinHook.h>
#include "EventsHooks.hpp"

namespace Hooks {
	void Initialize() {
		MH_Initialize();

		EventsHooks::Initialize();

	}
}