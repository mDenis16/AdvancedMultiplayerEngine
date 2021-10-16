#include "pch.h"
#include "VmtHook.hpp"
#include "ScriptHook.hpp"
#include "crossmap.hpp"
#include "script_program.hpp"
#include "natives.hpp"
#include "Functions.hpp"

namespace RAGE {
	inline std::unordered_map<scrProgram*, script_hook*> script_hook::s_map;

	static bool map_native(RAGE::ScrNativeHash* hash)
	{
		for (auto const& mapping : CrossMapList)
		{
			if (mapping.first == *hash)
			{
				*hash = mapping.second;
				return true;
			}
		}

		return false;
	}

	script_hook::script_hook(joaat_t script_hash, std::unordered_map<ScrNativeHash, ScrNativeHandler> native_replacements) :
		m_script_hash(script_hash),
		m_native_replacements(std::move(native_replacements))
	{
		ensure();
	}

	script_hook::~script_hook()
	{
		if (m_program)
		{
			for (auto [hash, handler_ptr] : m_native_handler_ptrs)
			{
				auto og_handler = Functions::m_get_native_handler(Functions::m_native_registration_table, hash);
				*handler_ptr = og_handler;
			}
		}

		if (m_vmt_hook)
		{
			m_vmt_hook->disable();
			s_map.erase(m_program);
		}
	}

	void script_hook::ensure()
	{
		if (m_vmt_hook)
			return;

		if (auto program = Functions::m_script_program_table->find_script(m_script_hash))
		{
			if (program->is_valid())
			{
				hook_instance(program);
				std::cout << "Hooked " << program->m_name << " script (" << (std::uint32_t)(static_cast<void*>(program)) << ")";
			}
		}
	}

	void script_hook::hook_instance(scrProgram* program)
	{
		m_program = program;
		s_map.emplace(m_program, this);
		m_vmt_hook = std::make_unique<vmt_hook>(m_program, 3);
		m_vmt_hook->hook(0, &scrprogram_dtor);

		for (auto [replacement_hash, replacement_handler] : m_native_replacements)
		{
			auto hash = replacement_hash;
			map_native(&hash);

			auto og_handler = Functions::m_get_native_handler(Functions::m_native_registration_table, hash);
			if (!og_handler)
				continue;

			auto handler_ptr = m_program->get_address_of_native_entrypoint(og_handler);
			if (!handler_ptr)
				continue;

			m_native_handler_ptrs.emplace(hash, reinterpret_cast<ScrNativeHandler*>(handler_ptr));
			*handler_ptr = replacement_handler;
		}
	}

	void script_hook::scrprogram_dtor(scrProgram* this_, bool free_memory)
	{
		if (auto it = s_map.find(this_); it != s_map.end())
		{
			auto hook = it->second;

			hook->m_program = nullptr;
			s_map.erase(it);

			auto og_func = hook->m_vmt_hook->get_original<decltype(&scrprogram_dtor)>(0);
			hook->m_vmt_hook->disable();
			hook->m_vmt_hook.reset();

			og_func(this_, free_memory);
		}
	}
}
