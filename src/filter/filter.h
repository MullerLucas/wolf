#pragma once

#include <vector>
#include <string>

#include "../utils.h"

namespace wolf {

// ----------------------------------------------

class Filter {
public:
    using Input  = std::vector<std::string>;
    using Output = std::vector<const std::string*>;

    virtual ~Filter() = default;

    virtual const Output& filter(const std::string& pattern) = 0;
};

// ----------------------------------------------

}

