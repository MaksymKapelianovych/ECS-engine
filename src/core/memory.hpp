#pragma once

#include "common.hpp"
// #include "platform/platformMemory.hpp"
#define GENERIC_MEMORY_SMALL_MEMSWAP_MAX 16
/**
 * Various memory functions. 
 *
 * Rather than use C's memory functions directly, they are encapsulated in a
 * this struct so they can potentially be optimized for various platforms.
 *
 * Additionally, this struct contains a few memory functions that are not part
 * of C's memory library.
 */
struct Memory
{
	// static inline void* memmove(void* dest, const void* src, uintptr_t amt)
	// {
	// 	return PlatformMemory::memmove(dest, src, amt);
	// }
	//
	// static inline int32 memcmp(const void* dest, const void* src, uintptr_t amt)
	// {
	// 	return PlatformMemory::memcmp(dest, src, amt);
	// }
	//
	// template<typename T>
	// static inline void* memset(void* dest, T val, uintptr_t amt)
	// {
	// 	return PlatformMemory::memset(dest, val, amt);
	// }
	//
	// static inline void* memzero(void* dest, uintptr_t amt)
	// {
	// 	return PlatformMemory::memset(dest, 0, amt);
	// }
	//
	// static inline void* memcpy(void* dest, const void* src, uintptr_t amt)
	// {
	// 	return PlatformMemory::memcpy(dest, src, amt);
	// }
	//
	// static inline void memswap(void* a, void* b, uintptr_t size)
	// {
	// 	return PlatformMemory::memswap(a, b, size);
	// }
	//
	enum 
	{
		DEFAULT_ALIGNMENT = 16,
		MIN_ALIGNMENT = 8
	};
	//
	// template<typename T>
	// static inline CONSTEXPR T align(const T ptr, uintptr_t alignment)
	// {
	// 	return PlatformMemory::align(ptr, alignment);
	// }
	//
	// static inline void* malloc(uintptr_t amt, uint32_t alignment=DEFAULT_ALIGNMENT)
	// {
	// 	return PlatformMemory::malloc(amt, alignment);
	// }
	//
	// static inline void* realloc(void* ptr, uintptr_t amt, uint32_t alignment=DEFAULT_ALIGNMENT)
	// {
	// 	return PlatformMemory::realloc(ptr, amt, alignment);
	// }
	//
	// static inline void* free(void* ptr)
	// {
	// 	return PlatformMemory::free(ptr);
	// }
	//
	// static inline uintptr_t getAllocSize(void* ptr)
	// {
	// 	return PlatformMemory::getAllocSize(ptr);
	// }

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

	static void* malloc(uintptr_t amt, uint32_t alignment=DEFAULT_ALIGNMENT);
	static void* realloc(void* ptr, uintptr_t amt, uint32_t alignment=DEFAULT_ALIGNMENT);
	static void* free(void* ptr);
	static uintptr_t getAllocSize(void* ptr);
private:
	static void bigmemswap(void* a, void* b, uintptr_t size);
	static void smallmemswap(void* a, void* b, uintptr_t size)
	{
		assertCheck(size <= GENERIC_MEMORY_SMALL_MEMSWAP_MAX);
		char temp_data[GENERIC_MEMORY_SMALL_MEMSWAP_MAX];
		void* temp = (void*)&temp_data;
		Memory::memcpy(temp, a, size);
		Memory::memcpy(a, b, size);
		Memory::memcpy(b, temp, size);
	}
};

template<>
FORCEINLINE void* Memory::memset(void* dest, uint8_t val, uintptr_t amt)
{
	return ::memset(dest, val, amt);
};
