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

    void insert_all(const std::vector<std::string> *input);

    void push(const std::string &prefix);
    const std::vector<const std::string*> &collect() const;

private:
    static constexpr usize WORKLOAD_MULTIPLIER = 3;

    const usize thread_count_;
    ThreadPool  pool_;

    const std::vector<std::string>  *input_;
    std::vector<const std::string*> output_;
    std::string                     prefix_;

    static void process_workload(std::vector<const std::string*> &output,
                                 usize start, usize end, const std::string &prefix,
                                 usize offset);
};

// ----------------------------------------------

}
