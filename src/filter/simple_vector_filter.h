#pragma once

#include <string>
#include <vector>

#include "../utils.h"
#include "../thread_pool.h"

namespace wolf {

// ----------------------------------------------

class SimpleVectorFilter {
public:
    SimpleVectorFilter(usize thread_count);

    void insert_all(const std::vector<std::string>* input);
    void filter(const std::string& prefix);
    const std::vector<const std::string*>& create_output() const;

private:
    const usize thread_count_;
    ThreadPool  pool_;

    const std::vector<std::string>* input_;
    std::vector<const std::string*> output_;
    std::string                     prefix_;

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
