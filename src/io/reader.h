#pragma once

#include <iostream>
#include <fstream>
#include <vector>

namespace wolf {

// ----------------------------------------------

class Reader {
public:
    virtual ~Reader() = default;

    virtual std::vector<std::string> read_lines() = 0;
};

// ----------------------------------------------

}

