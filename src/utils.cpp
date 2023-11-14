#include "utils.h"

#include <algorithm>
#include <bits/chrono.h>
#include <cwchar>
#include <iostream>
#include <vector>
#include <cstdarg>


namespace wolf {

// ----------------------------------------------

void log_info(const char* format, ...) {
    fprintf(stderr, "[INFO]: ");

    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
}

// ----------------------------------------------

b8 string_starts_with(const std::string& str, const std::string& prefix, usize offset) {
    if (prefix.size() > str.size()) {
        return false;
    }

    for (usize i = offset; i < prefix.size(); i++) {
        if (str[i] != prefix[i]) {
            return false;
        }
    }

    return true;
}

// ----------------------------------------------

Timer::Timer() : start_time_(std::chrono::high_resolution_clock::now()), end_time_(start_time_)
{}

void Timer::restart() {
    start_time_ = std::chrono::high_resolution_clock::now();
    end_time_   = start_time_;
}

void Timer::stop() {
    end_time_ = std::chrono::high_resolution_clock::now();
}

std::chrono::milliseconds Timer::elapsed_ms() const {
    return std::chrono::duration_cast<std::chrono::milliseconds>(end_time_ - start_time_);
}

std::chrono::microseconds Timer::elapsed_us() const {
    return std::chrono::duration_cast<std::chrono::microseconds>(end_time_ - start_time_);
}

// ----------------------------------------------

}
