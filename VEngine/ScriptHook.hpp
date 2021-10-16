#pragma once
#include "jooat.hpp"
#include "script_program.hpp"

namespace RAGE
{
	class script_hook
	{
	public:
		explicit script_hook(joaat_t script_hash, std::unordered_map<ScrNativeHash, ScrNativeHandler> native_replacements);
		~script_hook();

		void ensure();
	private:
		void hook_instance(scrProgram* program);

		static void scrprogram_dtor(scrProgram* this_, bool free_memory);
		static std::unordered_map<scrProgram*, script_hook*> s_map;

		joaat_t m_script_hash;
		scrProgram* m_program;
		std::unique_ptr<vmt_hook> m_vmt_hook;
		std::unordered_map<ScrNativeHash, ScrNativeHandler> m_native_replacements;
		std::unordered_map<ScrNativeHash, ScrNativeHandler*> m_native_handler_ptrs;
	};
}