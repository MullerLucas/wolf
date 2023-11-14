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

    virtual void reset()                            = 0;
    virtual void filter(const std::string& pattern) = 0;
    virtual const Output& create_output() const     = 0;
};

// ----------------------------------------------

}

