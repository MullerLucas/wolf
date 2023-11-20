#pragma once

#include <functional>
#include <vector>

#include "io/writer.h"
#include "utils.h"

namespace wolf {

// ----------------------------------------------

struct BenchResult {
    i64 construct_time  = std::numeric_limits<i64>::max();
    i64 session_time    = std::numeric_limits<i64>::max();
    i64 insert_time     = std::numeric_limits<i64>::max();
    i64 filter_time     = std::numeric_limits<i64>::max();
    i64 collect_time    = std::numeric_limits<i64>::max();
    i64 destruct_time   = std::numeric_limits<i64>::max();
};

// ----------------------------------------------

class Benchmark {
public:
    Benchmark(Writer &writer, const std::vector<std::string> &input,
              const std::vector<std::string> &prefixes,
              std::vector<usize> thread_count, usize iterations);

    void run() const;

private:
    Writer                          &writer_;
    const std::vector<std::string>  &input_;
    std::vector<std::string>        prefixes_;
    std::vector<usize>              thread_count_;
    usize                           iterations_;

    void run_any(
        std::function<BenchResult(const std::vector<std::string> &input,
                                  const std::string &prefix, usize thread_count)> fn
    ) const;

    static BenchResult run_vector_filter(const std::vector<std::string> &input,
                                         const std::string &prefix, usize thread_count);
    static BenchResult run_trie_filter(const std::vector<std::string>& input,
                                       const std::string& prefix, usize thread_count);
};

// ----------------------------------------------

}
