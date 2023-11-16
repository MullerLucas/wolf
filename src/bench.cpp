#include <algorithm>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <limits>
#include <memory>
#include <pstl/glue_execution_defs.h>

#include "bench.h"
#include "filter/simple_vector_filter.h"
#include "filter/trie_filter.h"
#include "utils.h"

namespace wolf {

Bench::Bench(
    Writer& writer,
    const std::vector<std::string>& input,
    const std::vector<std::string>& prefixes,
    std::vector<usize> num_threads,
    usize iterations
)
    : writer_(writer), input_(input), prefixes_(prefixes), num_threads_(num_threads), iterations_(iterations)
{}

void Bench::run() const {
    char time_buffer[80];
    {
        std::time_t time = std::time(nullptr);
        std::tm local_time = *std::localtime(&time);
        std::strftime(time_buffer, 80, "%Y-%m-%d %H:%M:%S", &local_time);
    }

    writer_ << "Benchmark (" << time_buffer << ")\n"
    << "Input:      " << input_.size() << " words\n"
    << "Iterations: " << iterations_ << "\n"
    << "Threads:    ";
    for (const auto& nt : num_threads_) {
        writer_ << nt << " ";
    }
    writer_ << "\n\n";


    log_info("Running Simple Vector Filter benchmarks\n");
    writer_ << "Simple Vector Filter:\n";
    run_any(run_vector_filter);

    // log_info("Running Trie-Filter benchmarks\n");
    // writer_ << "\nTrie Filter:\n";
    // run_any(run_trie_filter);
}

void Bench::run_any(
    std::function<BenchResult(const std::vector<std::string>& input, const std::string& prefix, usize num_threads)> fn
) const {
    usize remaining = prefixes_.size() * num_threads_.size();

    writer_.set_width(8);
    for (const auto& p : prefixes_) {
        for (auto nt : num_threads_) {
            log_info("%i sets remaining\n", remaining--);

            i64 best_construction_time = std::numeric_limits<i64>::max();
            i64 best_setup_time        = std::numeric_limits<i64>::max();
            i64 best_filter_time       = std::numeric_limits<i64>::max();

            for (usize i = 0; i < iterations_; ++i) {
                auto result = fn(input_, p, nt);
                best_construction_time = std::min(best_construction_time, result.construction_time);
                best_setup_time        = std::min(best_setup_time,        result.setup_time);
                best_filter_time       = std::min(best_filter_time,       result.filter_time);
            }

            i64 best_total_time = best_construction_time + best_setup_time + best_filter_time;

            writer_
            << "P: "   << p
            << " T: "   << nt
            << " SUM: " << best_total_time
            << " C: "   << best_construction_time
            << " S: "   << best_setup_time
            << " F: "   << best_filter_time
            << "\n";
        }
    }
    writer_.set_width(0);
}

BenchResult Bench::run_vector_filter(
    const std::vector<std::string>& input,
    const std::string& prefix,
    usize num_threads
) {
    BenchResult result;
    Timer t;

    t.restart();
    SimpleVectorFilter filter(num_threads);
    t.stop();
    result.construction_time = t.elapsed_us().count();

    t.restart();
    filter.init_data(&input);
    t.stop();

    // TODO (lm)
    result.setup_time = t.elapsed_us().count();

    t.restart();
    filter.filter(prefix);
    t.stop();
    result.filter_time = t.elapsed_us().count();

    return result;
}

BenchResult Bench::run_trie_filter(
    const std::vector<std::string>& input,
    const std::string& prefix,
    usize num_threads __attribute__((unused))
) {
    BenchResult result;
    Timer t;

    t.restart();
    TrieFilter filter(&input);
    t.stop();
    result.construction_time = t.elapsed_us().count();

    // TODO (lm)
    result.setup_time = 0;

    t.restart();
    filter.filter(prefix);
    t.stop();
    result.filter_time = t.elapsed_us().count();

    return result;
}

}
