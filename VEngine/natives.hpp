#pragma once
#include <cstdint>
#include <utility>
#include <glm\glm.hpp>

namespace RAGE
{
	class ScrNativeCallContext
	{
	public:
		void Reset()
		{
			ArgCount = 0;
			DataCount = 0;
		}

		template <typename T>
		void PushArg(T&& value)
		{
			static_assert(sizeof(T) <= sizeof(std::uint64_t));
			*reinterpret_cast<std::remove_cv_t<std::remove_reference_t<T>>*>(reinterpret_cast<std::uint64_t*>(Args) + (ArgCount++)) = std::forward<T>(value);
		}

		template <typename T>
		T& GetArg(std::size_t index)
		{
			static_assert(sizeof(T) <= sizeof(std::uint64_t));
			return *reinterpret_cast<T*>(reinterpret_cast<std::uint64_t*>(Args) + index);
		}

		template <typename T>
		void SetArg(std::size_t index, T&& value)
		{
			static_assert(sizeof(T) <= sizeof(std::uint64_t));
			*reinterpret_cast<std::remove_cv_t<std::remove_reference_t<T>>*>(reinterpret_cast<std::uint64_t*>(Args) + index) = std::forward<T>(value);
		}

		template <typename T>
		T* GetReturnValue()
		{
			return reinterpret_cast<T*>(ReturnVal);
		}

		template <typename T>
		void set_return_value(T&& value)
		{
			*reinterpret_cast<std::remove_cv_t<std::remove_reference_t<T>>*>(ReturnVal) = std::forward<T>(value);
		}
	protected:
		void* ReturnVal;
		std::uint32_t ArgCount;
		void* Args;
		std::int32_t DataCount;
		std::uint32_t Data[48];
	};

	using ScrNativeHash = std::uint64_t;
	using ScrNativeMapping = std::pair<ScrNativeHash, ScrNativeHash>;
	using ScrNativeHandler = void(*)(ScrNativeCallContext*);

	class ScrNativeRegistration;

#pragma pack(push, 1)
	class ScrNativeRegistrationTable
	{
		ScrNativeRegistration* Entries[0xFF];
		std::uint32_t Unknown;
		bool Initialized;
	};
#pragma pack(pop)

	//assert(sizeof(scrNativeCallContext) == 0xE0);
}

using NativeVoid = void;
using NativeAny = std::uint32_t;
using NativeHash = std::uint32_t;
using NativeEntity = std::int32_t;
using NativePlayer = std::int32_t;
using NativeFireId = std::int32_t;
using NativeInterior = std::int32_t;
using NativePed = NativeEntity;
using NativeVehicle = NativeEntity;
using NativeCam = std::int32_t;
using NativeObject = NativeEntity;
using NativePickup = NativeObject;
using NativeBlip = std::int32_t;
using NativeCamera = NativeEntity;
using NativeScrHandle = NativeEntity;
using NativeVector3 = glm::vec3;
