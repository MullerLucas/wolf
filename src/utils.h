#include <cstdint>
#include <iostream>
#include <vector>
#include <assert.h>

// ----------------------------------------------

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef size_t usize;

typedef int32_t b32;

// ----------------------------------------------

namespace wolf {

void log_info(const char* format, ...);

}

// ----------------------------------------------

namespace wolf {

std::vector<std::string> read_stdin();

}

// ----------------------------------------------
