#pragma once

#include "common.hpp"
#include "platform/platformMemory.hpp"
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
	static inline void* memmove(void* dest, const void* src, uintptr_t amt)
	{
		return PlatformMemory::memmove(dest, src, amt);
	}
	
	static inline int32_t memcmp(const void* dest, const void* src, uintptr_t amt)
	{
		return PlatformMemory::memcmp(dest, src, amt);
	}
	
	template<typename T>
	static inline void* memset(void* dest, T val, uintptr_t amt)
	{
		return PlatformMemory::memset(dest, val, amt);
	}
	
	static inline void* memzero(void* dest, uintptr_t amt)
	{
		return PlatformMemory::memset(dest, 0, amt);
	}
	
	static inline void* memcpy(void* dest, const void* src, uintptr_t amt)
	{
		return PlatformMemory::memcpy(dest, src, amt);
	}
	
	static inline void memswap(void* a, void* b, uintptr_t size)
	{
		return PlatformMemory::memswap(a, b, size);
	}
	
	enum 
	{
		DEFAULT_ALIGNMENT = 16,
		MIN_ALIGNMENT = 8
	};
	
	template<typename T>
	static inline CONSTEXPR T align(const T ptr, uintptr_t alignment)
	{
		return PlatformMemory::align(ptr, alignment);
	}
	
	static inline void* malloc(uintptr_t amt, uint32_t alignment=DEFAULT_ALIGNMENT)
	{
		return PlatformMemory::malloc(amt, alignment);
	}
	
	static inline void* realloc(void* ptr, uintptr_t amt, uint32_t alignment=DEFAULT_ALIGNMENT)
	{
		return PlatformMemory::realloc(ptr, amt, alignment);
	}
	
	static inline void* free(void* ptr)
	{
		return PlatformMemory::free(ptr);
	}
	
	static inline uintptr_t getAllocSize(void* ptr)
	{
		return PlatformMemory::getAllocSize(ptr);
	}

};

template<>
FORCEINLINE void* Memory::memset(void* dest, uint8_t val, uintptr_t amt)
{
	return ::memset(dest, val, amt);
};
