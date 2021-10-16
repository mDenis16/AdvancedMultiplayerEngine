#pragma once
#include <cstddef>
#include <cstdint>
#include <type_traits>

namespace Memory
{
	class Handle
	{
	public:
		Handle(void* ptr = nullptr);
		explicit Handle(std::uintptr_t ptr);

		template <typename T>
		std::enable_if_t<std::is_pointer_v<T>, T> As();

		template <typename T>
		std::enable_if_t<std::is_lvalue_reference_v<T>, T> As();

		template <typename T>
		std::enable_if_t<std::is_same_v<T, std::uintptr_t>, T> As();

		template <typename T>
		Handle Add(T offset);

		template <typename T>
		Handle Sub(T offset);

		Handle Rip();

		explicit operator bool();

		friend bool operator==(Handle a, Handle b);
		friend bool operator!=(Handle a, Handle b);
	private:
		void* Ptr;
	};

	inline Handle::Handle(void* ptr) :
		Ptr(ptr)
	{}

	inline Handle::Handle(std::uintptr_t ptr) :
		Ptr(reinterpret_cast<void*>(ptr))
	{}

	template <typename T>
	inline std::enable_if_t<std::is_pointer_v<T>, T> Handle::As()
	{
		return static_cast<T>(Ptr);
	}

	template <typename T>
	inline std::enable_if_t<std::is_lvalue_reference_v<T>, T> Handle::As()
	{
		return *static_cast<std::add_pointer_t<std::remove_reference_t<T>>>(Ptr);
	}

	template <typename T>
	inline std::enable_if_t<std::is_same_v<T, std::uintptr_t>, T> Handle::As()
	{
		return reinterpret_cast<std::uintptr_t>(Ptr);
	}

	template <typename T>
	inline Handle Handle::Add(T offset)
	{
		return handle(as<std::uintptr_t>() + offset);
	}

	template <typename T>
	inline Handle Handle::Sub(T offset)
	{
		return Handle(As<std::uintptr_t>() - offset);
	}

	inline Handle Handle::Rip()
	{
		return Add(As<std::int32_t&>()).Add(4);
	}

	inline bool operator==(Handle a, Handle b)
	{
		return a.Ptr == b.Ptr;
	}

	inline bool operator!=(Handle a, Handle b)
	{
		return a.Ptr != b.Ptr;
	}

	inline Handle::operator bool()
	{
		return Ptr != nullptr;
	}
}