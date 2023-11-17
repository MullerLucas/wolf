#pragma once

#include <string>
#include <vector>

#include "../utils.h"
#include "../thread_pool.h"

namespace wolf {

// ----------------------------------------------

class SimpleVectorFilter {
public:
    SimpleVectorFilter(usize num_threads);

    void init_data(const std::vector<std::string>* input);
    void filter(const std::string& prefix);
    const std::vector<const std::string*>& create_output() const;

private:
    const std::vector<std::string>* input_;
    std::vector<const std::string*> output_;

    ThreadPool  pool_;
    std::string prefix_;
    const usize num_threads_;

    static void process_workload(
        std::vector<const std::string*>& output,
        usize   start,
        usize   end,
        const   std::string& prefix,
        usize   offset
    );
};

// ----------------------------------------------

}
