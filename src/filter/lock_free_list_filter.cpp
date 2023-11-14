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
    : input(input), pattern_(""), num_threads(num_threads)
{
    reset();
}

void LockFreeListFilter::reset() {
    assert(this->input != nullptr);

    output.clear();
    output.reserve(input->size());

    for (const auto& in : *input) {
        output.push_back(&in);
    }

    pattern_.clear();
}

void LockFreeListFilter::filter(const std::string& pattern) {
    const usize size = output.size();
    usize chunck_size = size / num_threads;

    const usize offset = pattern_.size();
    pattern_ += pattern;

    log_info("Parallel filter with %d threads and chucks of size %i - offset is %i\n",
             num_threads, chunck_size, offset);

    std::vector<std::thread> threads;
    threads.reserve(num_threads);

    for (usize i = 0; i < num_threads; i++) {
        usize start = i * chunck_size;
        usize end = (i == num_threads - 1) ? size : (i + 1) * chunck_size;

        threads.emplace_back(
            LockFreeListFilter::process_workload,
            std::ref(output),
            start,
            end,
            std::ref(pattern_),
            offset
        );
    }

    for (auto& thread : threads) {
        thread.join();
    }

    auto it = std::remove_if(output.begin(), output.end(), [](const std::string* str) {
        return str == nullptr;
    });
    output.erase(it, output.end());
}

const LockFreeListFilter::Output& LockFreeListFilter::create_output() const {
    return output;
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
