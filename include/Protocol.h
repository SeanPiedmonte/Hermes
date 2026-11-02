#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>
#include <stdlib.h>

#ifndef L1D_CACHE_SIZE
    #if defined(__APPLE__) && defined(__aarch64__)
        #define L1D_CACHE_SIZE (64 * 1024)
    #elif defined(__x86_64__) || defined(_M_X64)
        #define L1D_CACHE_SIZE (32 * 1024)
    #elif defined(__aarch64__) || defined(_M_ARM64)
        #define L1D_CACHE_SIZE (32 * 1024)
    #else
        #define L1D_CACHE_SIZE (32 * 1024)
    #endif
#endif

#ifndef CACHE_LINE_SIZE
    #if defined(__APPLE__) && defined(__aarch64__)
        #define CACHE_LINE_SIZE 128
    #else 
        #define CACHE_LINE_SIZE 64
    #endif
#endif

#define HOT_BUFFER_SIZE L1D_CACHE_SIZE / 2

// Easier to work with type definitions
typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t  i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

#endif
