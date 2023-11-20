#include <vector>

#include "../utils.h"
#include "writer.h"

namespace wolf {

// ----------------------------------------------

void Writer::write_lines(const std::vector<std::string> &data)
{
    for (const auto& d : data)
        *this << d << "\n";
}

void Writer::write_lines(const std::vector<const std::string*> &data)
{
    for (const auto &d : data)
        *this << *d << "\n";
}

Writer &Writer::set_width(usize width)
{
    width_ = width;
    return *this;
}

// ----------------------------------------------

}
