#pragma once

#include "core/common.hpp"
#include <cstring>

#define GENERIC_MEMORY_SMALL_MEMSWAP_MAX 16

struct GenericMemory
{
	static FORCEINLINE void* memmove(void* dest, const void* src, uintptr_t amt)
	{
		return ::memmove(dest, src, amt);
	}

	static FORCEINLINE int32_t memcmp(const void* dest, const void* src, uintptr_t amt)
	{
		return ::memcmp(dest, src, amt);
	}

	template<typename T>
	static FORCEINLINE void* memset(void* destIn, T val, uintptr_t amt)
	{
		T* dest = (T*)destIn;
		uintptr_t amtT = amt/sizeof(T);
		uintptr_t remainder = amt % sizeof(T);
		for(uintptr_t i = 0; i < amtT; ++i, ++dest) {
			memcpy(dest, &val, sizeof(T));
		}
		memcpy(dest, &val, remainder);
		return destIn;
	}

	static FORCEINLINE void* memzero(void* dest, uintptr_t amt)
	{
		return ::memset(dest, 0, amt);
	}

	static FORCEINLINE void* memcpy(void* dest, const void* src, uintptr_t amt)
	{
		return ::memcpy(dest, src, amt);
	}

	static void memswap(void* a, void* b, uintptr_t size)
	{
		if(size <= GENERIC_MEMORY_SMALL_MEMSWAP_MAX) {
			smallmemswap(a, b, size);
		} else {
			bigmemswap(a, b, size);
		}
	}

	template<typename T>
	static FORCEINLINE CONSTEXPR T align(const T ptr, uintptr_t alignment)
	{
		return (T)(((intptr_t)ptr + alignment - 1) & ~(alignment-1));
	}

	static void* malloc(uintptr_t amt, uint32_t alignment);
	static void* realloc(void* ptr, uintptr_t amt, uint32_t alignment);
	static void* free(void* ptr);
	static uintptr_t getAllocSize(void* ptr);
private:
	static void bigmemswap(void* a, void* b, uintptr_t size);
	static void smallmemswap(void* a, void* b, uintptr_t size)
	{
		assertCheck(size <= GENERIC_MEMORY_SMALL_MEMSWAP_MAX);
		char temp_data[GENERIC_MEMORY_SMALL_MEMSWAP_MAX];
		void* temp = (void*)&temp_data;
		GenericMemory::memcpy(temp, a, size);
		GenericMemory::memcpy(a, b, size);
		GenericMemory::memcpy(b, temp, size);
	}
};

template<>
FORCEINLINE void* GenericMemory::memset(void* dest, uint8_t val, uintptr_t amt)
{
	return ::memset(dest, val, amt);
}

