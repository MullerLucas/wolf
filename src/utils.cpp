#include <iostream>
#include <vector>

// ----------------------------------------------

namespace wolf {

void log_info(const char* msg) {
    std::cerr << "[INFO]: " << msg << std::endl;
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

