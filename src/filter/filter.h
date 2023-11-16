#pragma once

#include <vector>
#include <string>

#include "../utils.h"

namespace wolf {

// ----------------------------------------------

class Filter {
public:
    using InVec  = std::vector<std::string>;
    using OutVec = std::vector<const std::string*>;

    // virtual ~Filter() = default;
    //
    // virtual void filter(const std::string& prefix) = 0;
    // virtual const std::vector<const std::string*>& create_output() const    = 0;
};

// ----------------------------------------------

}

