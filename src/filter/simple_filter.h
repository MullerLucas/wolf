#pragma once

#include <string>
#include <vector>

#include "../utils.h"
#include "filter.h"

namespace wolf {

// ----------------------------------------------

class LockFreeListFilter : public Filter {
public:
    LockFreeListFilter(const Input* input, usize num_threads);

    const Output& filter(const std::string& pattern) override;

private:
    const Input* input;
    Output output;
    usize num_threads;

    static void process_workload(
        Output& output,
        const std::string& pattern,
        usize start,
        usize end
    );
};

// ----------------------------------------------

}
