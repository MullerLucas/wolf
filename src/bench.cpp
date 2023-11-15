#include <algorithm>
#include <iostream>
#include <limits>
#include <memory>

#include "bench.h"
#include "filter/lock_free_list_filter.h"
#include "filter/trie_filter.h"

namespace wolf {

Bench::Bench(
    Writer& writer,
    const std::vector<std::string>& input,
    std::vector<usize> num_threads,
    usize iterations
)
    : writer_(writer), input_(input), num_threads_(num_threads), iterations_(iterations)
{}

void Bench::run() const {
    run_vector_filter();
    run_trie_filter();
}

void Bench::run_vector_filter() const {
    Timer t;

    for (const auto& nt : num_threads_) {
        i64 best_creation_time = std::numeric_limits<i64>::max();
        i64 best_filter_time   = std::numeric_limits<i64>::max();

        for (usize i = 0; i < iterations_; i++) {
            t.restart();
            auto filter = std::make_unique<LockFreeListFilter>(&input_, nt);
            t.stop();
            best_creation_time = std::min(t.elapsed_us().count(), best_creation_time);

            t.restart();
            filter->filter("AA");
            t.stop();
            best_filter_time = std::min(t.elapsed_us().count(), best_filter_time);
        }

        writer_ << "[VectorFilter] T: " << nt << ", C: " << best_creation_time << "us, F: " << best_filter_time << "us\n";
    }
}

void Bench::run_trie_filter() const {
    Timer t;

    for (const auto& nt : num_threads_) {
        i64 best_creation_time = std::numeric_limits<i64>::max();
        i64 best_filter_time   = std::numeric_limits<i64>::max();

        for (usize i = 0; i < iterations_; i++) {
            t.restart();
            auto filter = std::make_unique<TrieFilter>(&input_);
            t.stop();
            best_creation_time = std::min(t.elapsed_us().count(), best_creation_time);

            t.restart();
            filter->filter("AA");
            t.stop();
            best_filter_time = std::min(t.elapsed_us().count(), best_filter_time);
        }

        writer_ << "[TrieFilter] T: " << nt << " C: " << best_creation_time << "us, F: " << best_filter_time << "us\n";
    }
}

}
