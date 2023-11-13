#include <cwchar>
#include <iostream>
#include <vector>
#include <cstdarg>

// ----------------------------------------------

namespace wolf {

void log_info(const char* format, ...) {
    fprintf(stderr, "[INFO]: ");

    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
}

}

