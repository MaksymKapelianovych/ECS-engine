#pragma once

#include <cassert>
#include <stdio.h>
#include <stdexcept>
#include "platform/platform.hpp"

//Include known-size integer files, based on compiler. Some compilers do not have these
//files, so they must be created manually.
#if defined(__GNUC__) || defined(__clang__) || (defined(_MSC_VER) && _MSC_VER >= 1600)
	#include <stdint.h>
#elif defined(_MSC_VER)
	typedef signed   __int8  int8_t;
	typedef unsigned __int8  uint8_t;
	typedef signed   __int16 int16_t;
	typedef unsigned __int16 uint16_t;
	typedef signed   __int32 int32_t;
	typedef unsigned __int32 uint32_t;
	typedef signed   __int64 int64_t;
	typedef unsigned __int64 uint64_t;
	typedef uint64_t uintptr_t;
	typedef int64_t intptr_t;
	typedef int16_t wchar_t;
#else
	// TODO: Have a better "if all else fails" condition.
	//
	// Preferably, make all sizes at least big enough for the data size.
	// Also, try including C++11 standard types if possible.
	//
	// List of C++'s standard data sizing rules:
	// 
	// sizeof(char) == 1
	// sizeof(char) <= sizeof(short)
	// sizeof(short) <= sizeof(int)
	// sizeof(int) <= sizeof(long)
	// sizeof(long) <= sizeof(long long)
	// sizeof(char) * CHAR_BIT >= 8
	// sizeof(short) * CHAR_BIT >= 16
	// sizeof(int) * CHAR_BIT >= 16
	// sizeof(long) * CHAR_BIT >= 32
	// sizeof(long long) * CHAR_BIT >= 64
	
	typedef signed   char      int8_t;
	typedef unsigned char      uint8_t;
	typedef signed   short int int16_t;
	typedef unsigned short int uint16_t;
	typedef signed   int       int32_t;
	typedef unsigned int       uint32_t;
	typedef long long          int64_t;
	typedef unsigned long long uint64_t;
	typedef uint64_t uintptr_t;
	typedef int64_t intptr_t;
	typedef int16_t wchar_t;
#endif

// typedef uint8_t CHART;
//
// typedef int8_t int8;
// typedef int16_t int16;
// typedef int32_t int32;
// typedef int64_t int64;
// typedef uint8_t uint8;
// typedef uint16_t uint16;
// typedef uint32_t uint32;
// typedef uint64_t uint64;
// typedef intptr_t intptr;
// typedef uintptr_t uintptr;

#ifdef DEBUG
	#define assertCheck assert
	#define DEBUG_LOG_ECS 1
#else
	#define assertCheck (void)
	#define DEBUG_LOG_ECS 0
#endif

#ifdef COMPILER_MSVC
	#define FORCEINLINE __forceinline
#elif defined(COMPILER_GCC) || defined(COMPILER_CLANG)
	#define FORCEINLINE inline __attribute__ ((always_inline))
#else
	#define FORCEINLINE inline
#endif

#if __cplusplus < 201103L
	// #define nullptr NULL
	#define CONSTEXPR
#else
	#define CONSTEXPR constexpr
#endif

#if _MSC_VER >= 1920
	#define PLATFORM_COMPILER_HAS_IF_CONSTEXPR 1
#else
	#define PLATFORM_COMPILER_HAS_IF_CONSTEXPR 0
#endif

#define NULL_COPY_AND_ASSIGN(T) \
	T(const T& other) {(void)other;} \
	void operator=(const T& other) { (void)other; }

#define M_CON(A, B) M_CON_(A, B)
#define M_CON_(A, B) A##B

#define STATIC_BLOCK \
	[[maybe_unused]] static const auto M_CON(_static_block,__LINE__) = []()


// HASH

// compile time FNV-1a
constexpr uint32_t Hash32_CT( const char * str, size_t n, uint32_t basis = UINT32_C( 2166136261 ) ) {
	return n == 0 ? basis : Hash32_CT( str + 1, n - 1, ( basis ^ str[ 0 ] ) * UINT32_C( 16777619 ) );
}

template< uint32_t id >
uint32_t constexpr typeid_helper() {
	return id;
}

template <typename T>
uint32_t constexpr type_hash();

#define DECLARE_HASH(Class) \
	template <> \
	uint32_t constexpr type_hash<Class>(){ \
		return typeid_helper< Hash32_CT( #Class, sizeof( #Class ) - 1 ) >(); \
	}


#define LOG_ERROR "Error"
#define LOG_WARNING "Warning"
#define LOG_INFO "Info"
#define LOG_TYPE_RENDERER "Renderer"
#define LOG_TYPE_IO "IO"
#define DEBUG_LOG(category, level, message, ...) \
	fprintf(stderr, "[%s] ", category); \
	fprintf(stderr, "[%s] (%s:%d): ", level, __FUNCTION__, __LINE__); \
	fprintf(stderr, message, ##__VA_ARGS__); \
	fprintf(stderr, "\n")
#define DEBUG_LOG_LITE(message, ...) \
	fprintf(stderr, message, ##__VA_ARGS__); \
	fprintf(stderr, "\n")
#define DEBUG_LOG_TEMP(message, ...) DEBUG_LOG("TEMP", "TEMP", message, ##__VA_ARGS__)
#define DEBUG_LOG_TEMP2(message) DEBUG_LOG("TEMP", "TEMP", "%s", message)
// #define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

