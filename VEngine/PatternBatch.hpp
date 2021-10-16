#pragma once
#include <functional>
#include <vector>
#include "Pattern.hpp"
#include "Handle.hpp"

namespace Memory
{
	class PatternBatch
	{
	public:
		explicit PatternBatch() = default;
		~PatternBatch() noexcept = default;

		void Add(std::string name, Pattern pattern, std::function<void(Handle)> callback);
		void Run(Range region);

		struct Entry
		{
			std::string mName;
			Pattern mPattern;
			std::function<void(Handle)> mCallback;

			explicit Entry(std::string name, Pattern pattern, std::function<void(Handle)> callback) :
				mName(std::move(name)),
				mPattern(std::move(pattern)),
				mCallback(std::move(callback))
			{}
		};

	private:
		std::vector<Entry> Entries;
	};
}
