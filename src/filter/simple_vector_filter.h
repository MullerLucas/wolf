#pragma once

#include <string>
#include <vector>

#include "../utils.h"
#include "filter.h"

namespace wolf {

// ----------------------------------------------

class SimpleVectorFilter : public Filter {
public:
    SimpleVectorFilter(const Input* input, usize num_threads);

    void filter(const std::string& prefix) override;
    const Output& create_output() const    override;

    void reset();

private:
    const Input* input_;
    Output       output_;
    std::string  prefix_;
    const usize  num_threads_;

    static void process_workload(
        Output& output,
        usize start,
        usize end,
        const std::string& prefix,
        usize offset
    );
};

// ----------------------------------------------

}
