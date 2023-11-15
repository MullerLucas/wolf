#pragma once

#include <functional>
#include <vector>

#include "io/writer.h"
#include "utils.h"

namespace wolf {

// ----------------------------------------------

struct BenchResult {
    i64 construction_time;
    i64 setup_time;
    i64 filter_time;
};

// ----------------------------------------------

class Bench {
public:
    Bench(
        Writer& writer,
        const std::vector<std::string>& input,
        const std::vector<std::string>& prefixes,
        std::vector<usize> num_threads,
        usize iterations
    );

    void run() const;

private:
    Writer& writer_;
    const std::vector<std::string>& input_;
    std::vector<std::string> prefixes_;
    std::vector<usize> num_threads_;
    usize iterations_;

    void run_any(
        std::function<BenchResult(const std::vector<std::string>& input, const std::string& prefix, usize num_threads)> fn
    ) const;

    static BenchResult run_vector_filter(
        const std::vector<std::string>& input,
        const std::string& prefix,
        usize num_threads
    );
    static BenchResult run_trie_filter(
        const std::vector<std::string>& input,
        const std::string& prefix,
        usize num_threads
    );
};

// ----------------------------------------------

}
