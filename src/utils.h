#pragma once

#include <chrono>
#include <cstdint>
#include <iostream>
#include <vector>
#include <assert.h>

// ----------------------------------------------

// TODO(lm): make sure that these types are correct
typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t  i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef float  f32;
typedef double f64;

typedef size_t usize;

typedef int8_t b8;

namespace wolf {

// ----------------------------------------------

void log_info(const char* format, ...);
b8 string_starts_with(const std::string& str, const std::string& prefix, usize offset);

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
