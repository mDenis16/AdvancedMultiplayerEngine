#include "pch.h"

#include "Handle.hpp"
#include "Range.hpp"
#include "Pattern.hpp"
#include "PatternBatch.hpp"



namespace Memory
{
	void PatternBatch::Add(std::string name, Pattern pattern, std::function<void(Handle)> callback)
	{
		Entries.emplace_back(std::move(name), std::move(pattern), std::move(callback));
	}

	void PatternBatch::Run(Range Region)
	{
		bool all_found = true;
		for (auto& entry : Entries)
		{
			if (auto result = Region.Scan(entry.mPattern))
			{
				if (entry.mCallback)
				{
					std::invoke(std::move(entry.mCallback), result);
				//	LOG(big::INFO_TO_FILE) << "Found '" << entry.m_name << "' GTA5.exe+" << HEX_TO_UPPER(result.as<DWORD64>() - region.begin().as<DWORD64>());
				}
				else
				{
					all_found = false;
					//LOG(WARNING) << "Failed to find '" << entry.m_name << "'.";
				}
			}
			else
			{
				all_found = false;
				//LOG(WARNING) << "Failed to find '" << entry.m_name << "'.";
			}
		}

		Entries.clear();
		if (!all_found)
		{
			throw std::runtime_error("Failed to find some patterns.");
		}
	}
}