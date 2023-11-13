#include "utils.h"

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

Timer::Timer() : start_time(std::chrono::high_resolution_clock::now()), end_time(start_time)
{}

void Timer::restart() {
    start_time = std::chrono::high_resolution_clock::now();
    end_time = start_time;
}

void Timer::stop() {
    end_time = std::chrono::high_resolution_clock::now();
}

std::chrono::milliseconds Timer::elapsed_ms() const {
    return std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
}

std::chrono::microseconds Timer::elapsed_us() const {
    return std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
}

// ----------------------------------------------

}
