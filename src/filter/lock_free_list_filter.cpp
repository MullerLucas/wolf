#include <algorithm>
#include <cassert>
#include <functional>
#include <string>
#include <thread>
#include <vector>

#include "../utils.h"
#include "lock_free_list_filter.h"

namespace wolf {

// ----------------------------------------------

LockFreeListFilter::LockFreeListFilter(const Input* input, usize num_threads)
    : input_(input), pattern_(""), num_threads_(num_threads)
{
    reset();
}

void LockFreeListFilter::reset() {
    assert(this->input_ != nullptr);

    output_.clear();
    output_.reserve(input_->size());

    for (const auto& in : *input_) {
        output_.push_back(&in);
    }

    pattern_.clear();
}

void LockFreeListFilter::filter(const std::string& pattern) {
    const usize size        = output_.size();
    const usize chunck_size = size / num_threads_;

    const usize offset = pattern_.size();
    pattern_ += pattern;

    std::vector<std::thread> threads;
    threads.reserve(num_threads_);

    for (usize i = 0; i < num_threads_; i++) {
        usize start = i * chunck_size;
        usize end   = (i == num_threads_ - 1) ? size : (i + 1) * chunck_size;

        threads.emplace_back(
            LockFreeListFilter::process_workload,
            std::ref(output_),
            start,
            end,
            std::ref(pattern_),
            offset
        );
    }

    for (auto& thread : threads) {
        thread.join();
    }

    auto it = std::remove_if(output_.begin(), output_.end(), [](const std::string* str) {
        return str == nullptr;
    });
    output_.erase(it, output_.end());
}

const LockFreeListFilter::Output& LockFreeListFilter::create_output() const {
    return output_;
}

void LockFreeListFilter::process_workload(
    Output& output,
    usize start,
    usize end,
    const std::string& pattern,
    usize offset
) {
    for (usize i = start; i < end; i++) {
        if (!string_starts_with(*output[i], pattern, offset)) {
            output[i] = nullptr;
        }
    }
}

// ----------------------------------------------

}
