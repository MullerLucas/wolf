#pragma once

#include <iostream>
#include <fstream>
#include <vector>

#include "reader.h"

namespace wolf {

// ----------------------------------------------

class ConsoleReader: public Reader {
public:
    std::vector<std::string> read_lines();
};

// ----------------------------------------------

}

