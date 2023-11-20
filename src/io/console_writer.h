#pragma once

#include "writer.h"

namespace wolf {

// ----------------------------------------------

class ConsoleWriter : public Writer {
public:
    Writer &operator<<(const std::string &message) override;
    Writer &operator<<(i64 number) override;
    Writer &operator<<(std::ostream &(*manipulator)(std::ostream&)) override;
};

// ----------------------------------------------

}
