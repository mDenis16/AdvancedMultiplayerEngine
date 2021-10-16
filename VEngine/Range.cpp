#include "pch.h"
#include "Handle.hpp"
#include "PatternBatch.hpp"
#include "Range.hpp"
#include "Pattern.hpp"



namespace Memory
{
    Range::Range(Handle base, std::size_t size) :
		Base(base), mSize(size)
	{
	}

	Handle Range::Begin()
	{
		return Base;
	}

	Handle Range::End()
	{
		return Base.Add(mSize);
	}

	std::size_t Range::Size()
	{
		return mSize;
	}

	bool Range::Contains(Handle h)
	{
		return h.As<std::uintptr_t>() >= Begin().As<std::uintptr_t>() && h.As<std::uintptr_t>() <= End().As<std::uintptr_t>();
	}

	static bool PatternMaches(std::uint8_t* target, const std::optional<std::uint8_t>* Sig, std::size_t Length)
	{
		for (std::size_t i = 0; i < Length; ++i)
			if (Sig[i] && *Sig[i] != target[i])
				return false;
		

		return true;
	};

	Handle Range::Scan(Pattern const& sig)
	{
		auto data = sig.Bytes.data();
		auto length = sig.Bytes.size();
		for (std::uintptr_t i = 0; i < Size() - length; ++i)
			if (PatternMaches(Base.Add(i).As<std::uint8_t*>(), data, length))
				return Base.Add(i);
		
		
		return nullptr;
	}

	std::vector<Handle> Range::ScanAll(Pattern const& sig)
	{
		std::vector<Handle> result;

		auto data = sig.Bytes.data();
		auto length = sig.Bytes.size();
		for (std::uintptr_t i = 0; i < Size() - length; ++i)
			if (PatternMaches(Base.Add(i).As<std::uint8_t*>(), data, length))
				result.push_back(Base.Add(i));
			
		

		return std::move(result);
	}
}