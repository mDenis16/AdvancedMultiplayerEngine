#pragma once
#include "gtaThread.h"
#include "SysMemAllocator.hpp"


namespace RAGE {
	class tlsContext
	{
	public:
		char m_padding1[0xC8];          // 0x00
		sysMemAllocator* m_allocator;   // 0xC8
		char m_padding2[0x760];         // 0xD0
		void* m_script_thread;     // 0x830
		bool m_is_script_thread_active; // 0x838

		static tlsContext* get()
		{
			return *reinterpret_cast<tlsContext**>(__readgsqword(0x58));
		}
	};
}
