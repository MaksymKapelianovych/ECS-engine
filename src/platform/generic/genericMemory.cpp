#include "genericMemory.hpp"
#include "math/math.hpp"
#include <cstdlib>
#include <stdio.h>

void* GenericMemory::malloc(uintptr_t amt, uint32_t alignment)
{
	alignment = Math::max(amt >= 16 ? 16u : 8u, alignment);
	void* ptr = ::malloc(amt + alignment + sizeof(void*) + sizeof(uintptr_t));
	void* result = align((uint8_t*)ptr + sizeof(void*) + sizeof(uintptr_t), (uintptr_t)alignment);
	*((void**)((uint8_t*)result - sizeof(void*))) = ptr;
	*((uintptr_t*)((uint8_t*)result - sizeof(void*) - sizeof(uintptr_t))) = amt;
	return result;
}

void* GenericMemory::realloc(void* ptr, uintptr_t amt, uint32_t alignment)
{
	alignment = Math::max(amt >= 16 ? 16u : 8u, alignment);
	if(ptr == nullptr) {
		return GenericMemory::malloc(amt, alignment);
	}

	if(amt == 0) {
		GenericMemory::free(ptr);
		return nullptr;
	}

	void* result = malloc(amt, alignment);
	uintptr_t size = GenericMemory::getAllocSize(ptr);
	GenericMemory::memcpy(result, ptr, Math::min(size, amt));
	free(ptr);

	return result;
}

void* GenericMemory::free(void* ptr)
{
	if(ptr) {
		::free(*((void**)((uint8_t*)ptr - sizeof(void*))));
	}
	return nullptr;
}

uintptr_t GenericMemory::getAllocSize(void* ptr)
{
	return *((uintptr_t*)((uint8_t*)ptr - sizeof(void*) - sizeof(uintptr_t)));
}

void GenericMemory::bigmemswap(void* a, void* b, uintptr_t size)
{
	uint64_t* ptr1 = (uint64_t*)a;
	uint64_t* ptr2 = (uint64_t*)b;
	while(size > GENERIC_MEMORY_SMALL_MEMSWAP_MAX) {
		uint64_t tmp = *ptr1;
		*ptr1 = *ptr2;
		*ptr2 = tmp;
		size -= 8;
		ptr1++;
		ptr2++;
	}
	smallmemswap(ptr1, ptr2, size);
}
