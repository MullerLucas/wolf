#pragma once

#include <vector>

#include "io/writer.h"
#include "utils.h"

namespace wolf {

class Bench {
public:
    Bench(
        Writer& writer,
        const std::vector<std::string>& input,
        std::vector<usize> num_threads,
        usize iterations
    );

    void run() const;

private:
    Writer& writer_;
    const std::vector<std::string>& input_;
    std::vector<usize> num_threads_;
    usize iterations_;

    void run_vector_filter() const;
    void run_trie_filter() const;
};

}
