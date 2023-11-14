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

    void reset()                            override;
    void filter(const std::string& pattern) override;
    const Output& create_output() const     override;

private:
    const Input* input_;
    Output       output_;
    std::string  pattern_;
    const usize  num_threads_;

    static void process_workload(
        Output& output,
        usize start,
        usize end,
        const std::string& pattern,
        usize offset
    );
};

// ----------------------------------------------

}
