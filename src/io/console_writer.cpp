#include <cassert>
#include <cstdio>
#include <iomanip>
#include <iostream>
#include <vector>

#include "../utils.h"
#include "writer.h"
#include "console_writer.h"

namespace wolf {

// ----------------------------------------------

Writer &ConsoleWriter::operator<<(std::ostream &(*manipulator)(std::ostream&))
{
    std::cout << manipulator;
    return *this;
}

Writer &ConsoleWriter::operator<<(const std::string &message)
{
    std::cout << std::setw(width_) << message;
    return *this;
}

Writer &ConsoleWriter::operator<<(i64 number)
{
    std::cout << std::setw(width_) << number;
    return *this;
}

// ----------------------------------------------

}
