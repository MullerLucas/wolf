#pragma once

#include <string>
#include <vector>

#include "../utils.h"
#include "filter.h"
#include "../thread_pool.h"

namespace wolf {

// ----------------------------------------------

class SimpleVectorFilter : public Filter {
public:
    SimpleVectorFilter(usize num_threads);

    void init_data(const std::vector<std::string>* input);
    void filter(const std::string& prefix);
    const std::vector<const std::string*>& create_output() const;


private:
    ThreadPool   pool_;
    const InVec* input_;
    OutVec       output_;
    std::string  prefix_;
    const usize  num_threads_;

    static void process_workload(
        OutVec& output,
        usize   start,
        usize   end,
        const   std::string& prefix,
        usize   offset
    );
};

// ----------------------------------------------

}
