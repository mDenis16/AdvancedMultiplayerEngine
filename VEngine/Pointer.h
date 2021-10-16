#pragma once
namespace Memory {
	class Pointer
	{
	public:
		Pointer(void* ptr = nullptr);
		explicit Pointer(std::uintptr_t ptr);

		template <typename T>
		std::enable_if_t<std::is_pointer_v<T>, T> as();

		template <typename T>
		std::enable_if_t<std::is_lvalue_reference_v<T>, T> as();

		template <typename T>
		std::enable_if_t<std::is_same_v<T, std::uintptr_t>, T> as();

		template <typename T>
		Pointer add(T offset);

		template <typename T>
		Pointer sub(T offset);

		Pointer rip();

		explicit operator bool();

		friend bool operator==(Pointer a, Pointer b);
		friend bool operator!=(Pointer a, Pointer b);
	private:
		void* ptr;
	};

	inline Pointer::Pointer(void* ptr) :
		ptr(ptr)
	{}

	inline Pointer::Pointer(std::uintptr_t ptr) :
		ptr(reinterpret_cast<void*>(ptr))
	{}

	template <typename T>
	inline std::enable_if_t<std::is_pointer_v<T>, T> Pointer::as()
	{
		return static_cast<T>(ptr);
	}

	template <typename T>
	inline std::enable_if_t<std::is_lvalue_reference_v<T>, T> Pointer::as()
	{
		return *static_cast<std::add_pointer_t<std::remove_reference_t<T>>>(ptr);
	}

	template <typename T>
	inline std::enable_if_t<std::is_same_v<T, std::uintptr_t>, T> Pointer::as()
	{
		return reinterpret_cast<std::uintptr_t>(ptr);
	}

	template <typename T>
	inline Pointer Pointer::add(T offset)
	{
		return Pointer(as<std::uintptr_t>() + offset);
	}

	template <typename T>
	inline Pointer Pointer::sub(T offset)
	{
		return Pointer(as<std::uintptr_t>() - offset);
	}

	inline Pointer Pointer::rip()
	{
		return add(as<std::int32_t&>()).add(4);
	}

	inline bool operator==(Pointer a, Pointer b)
	{
		return a.ptr == b.ptr;
	}

	inline bool operator!=(Pointer a, Pointer b)
	{
		return a.ptr != b.ptr;
	}

	inline Pointer::operator bool()
	{
		return ptr != nullptr;
	}
}