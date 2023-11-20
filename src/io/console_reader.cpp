#include <string>
#include <vector>

#include "../utils.h"
#include "console_reader.h"

namespace wolf {

// ----------------------------------------------

std::vector<std::string> ConsoleReader::read_lines()
{
    std::string tmp;
    std::vector<std::string> input;

    while (std::cin >> tmp)
        input.push_back(std::move(tmp));

    return input;
}

// ----------------------------------------------

}

