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

// ----------------------------------------------


namespace wolf
{

std::vector<std::string> read_stdin() {
    std::string tmp;
    std::vector<std::string> input;

    while (std::cin >> tmp) {
        input.push_back(std::move(tmp));
    }

    return input;
}

}

