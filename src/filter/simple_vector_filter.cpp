#include <algorithm>
#include <cassert>
#include <functional>
#include <string>
#include <thread>
#include <vector>

#include "../utils.h"
#include "simple_vector_filter.h"

namespace wolf {

// ----------------------------------------------

SimpleVectorFilter::SimpleVectorFilter(const Input* input, usize num_threads)
    : input_(input), prefix_(""), num_threads_(num_threads)
{
    reset();
}

void SimpleVectorFilter::reset() {
    assert(this->input_ != nullptr);

    output_.clear();
    output_.reserve(input_->size());

    for (const auto& in : *input_) {
        output_.push_back(&in);
    }

    prefix_.clear();
}

void SimpleVectorFilter::filter(const std::string& prefix) {
    const usize size        = output_.size();
    const usize chunck_size = size / num_threads_;

    const usize offset = prefix_.size();
    prefix_ += prefix;

    std::vector<std::thread> threads;
    threads.reserve(num_threads_);

    for (usize i = 0; i < num_threads_; i++) {
        usize start = i * chunck_size;
        usize end   = (i == num_threads_ - 1) ? size : (i + 1) * chunck_size;

        threads.emplace_back(
            SimpleVectorFilter::process_workload,
            std::ref(output_),
            start,
            end,
            std::ref(prefix_),
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

const SimpleVectorFilter::Output& SimpleVectorFilter::create_output() const {
    return output_;
}

void SimpleVectorFilter::process_workload(
    Output& output,
    usize start,
    usize end,
    const std::string& prefix,
    usize offset
) {
    for (usize i = start; i < end; i++) {
        if (!string_starts_with(*output[i], prefix, offset)) {
            output[i] = nullptr;
        }
    }
}

// ----------------------------------------------

}
