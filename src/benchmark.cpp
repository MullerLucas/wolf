#include <algorithm>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <limits>
#include <memory>
#include <pstl/glue_execution_defs.h>

#include "benchmark.h"
#include "filter/multi_trie_filter.h"
#include "filter/simple_vector_filter.h"
#include "utils.h"

namespace wolf {

Benchmark::Benchmark(
    Writer& writer,
    const std::vector<std::string>& input,
    const std::vector<std::string>& prefixes,
    std::vector<usize> thread_count,
    usize iterations
)
    : writer_(writer), input_(input), prefixes_(prefixes),
        thread_count_(thread_count), iterations_(iterations)
{}

void Benchmark::run() const {
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
    for (const auto& nt : thread_count_) {
        writer_ << nt << " ";
    }
    writer_ << "\n\n";

    // log_info("Running-Simple-Vector-Filter benchmarks\n");
    // writer_ << "Simple-Vector-Filter:\n";
    // run_any(run_vector_filter);

    log_info("Running Multi-Trie-Filter benchmarks\n");
    writer_ << "\nMulti-Trie-Filter:\n";
    run_any(run_trie_filter);

}

void Benchmark::run_any(
    std::function<BenchResult(const std::vector<std::string>& input, const std::string& prefix, usize thread_count)> fn
) const {
    usize remaining = prefixes_.size() * thread_count_.size();

    writer_.set_width(8);
    for (const auto& p : prefixes_) {
        for (auto nt : thread_count_) {
            log_info("%i sets remaining\n", remaining--);

            i64 best_construct_time = std::numeric_limits<i64>::max();
            i64 best_session_time   = std::numeric_limits<i64>::max();
            i64 best_insert_time    = std::numeric_limits<i64>::max();
            i64 best_filter_time    = std::numeric_limits<i64>::max();
            i64 best_collect_time   = std::numeric_limits<i64>::max();

            for (usize i = 0; i < iterations_; ++i) {
                auto result = fn(input_, p, nt);
                best_construct_time = std::min(best_construct_time, result.construct_time);
                best_session_time   = std::min(best_session_time,   result.session_time);
                best_insert_time    = std::min(best_insert_time,    result.insert_time);
                best_filter_time    = std::min(best_filter_time,    result.filter_time);
                best_collect_time   = std::min(best_collect_time,   result.collect_time);
            }

            i64 best_total_time = best_construct_time + best_session_time + best_insert_time
                + best_filter_time + best_collect_time;

            writer_
            << "Pre: "  << p
            << " Ttr: " << nt
            << " All: " << best_total_time
            << " Con: " << best_construct_time
            << " Ses: " << best_session_time
            << " Ins: " << best_insert_time
            << " Fil: " << best_filter_time
            << " Col: " << best_collect_time
            << "\n";
        }
    }
    writer_.set_width(0);
}

BenchResult Benchmark::run_vector_filter(
    const std::vector<std::string>& input,
    const std::string& prefix,
    usize thread_count
) {
    BenchResult result;
    Timer t;

    t.restart();
    SimpleVectorFilter filter(thread_count);
    t.stop();
    result.construct_time = t.elapsed_us().count();

    t.restart();
    // TODO
    t.stop();
    result.session_time = t.elapsed_us().count();

    t.restart();
    filter.insert_all(&input);
    t.stop();
    result.insert_time = t.elapsed_us().count();

    t.restart();
    filter.filter(prefix);
    t.stop();
    result.filter_time = t.elapsed_us().count();

    t.restart();
    // TODO
    t.stop();
    result.collect_time = t.elapsed_us().count();

    return result;
}

BenchResult Benchmark::run_trie_filter(
    const std::vector<std::string>& input,
    const std::string& prefix,
    usize thread_count
) {
    BenchResult result;
    Timer t;

    t.restart();
    MultiTrieFilter filter(thread_count);
    t.stop();
    result.construct_time = t.elapsed_us().count();

    t.restart();
    auto session = filter.create_session();
    t.stop();
    result.session_time = t.elapsed_us().count();

    t.restart();
    filter.insert_all(input);
    t.stop();
    result.insert_time = t.elapsed_us().count();

    t.restart();
    filter.push_filter(session, prefix);
    t.stop();
    result.filter_time = t.elapsed_us().count();

    t.restart();
    filter.collect(session);
    t.stop();
    result.collect_time = t.elapsed_us().count();

    return result;
}

}
