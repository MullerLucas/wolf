#pragma once

#include <chrono>
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

namespace wolf {

// ----------------------------------------------

void log_info(const char* format, ...);
b32 string_starts_with(const std::string& str, const std::string& prefix, usize offset);

// ----------------------------------------------

class Timer {
public:
    Timer();

    void restart();
    void stop();
    std::chrono::milliseconds elapsed_ms() const;
    std::chrono::microseconds elapsed_us() const;

private:
    std::chrono::high_resolution_clock::time_point start_time_;
    std::chrono::high_resolution_clock::time_point end_time_;
};

// ----------------------------------------------
}
