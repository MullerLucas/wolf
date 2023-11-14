#include <algorithm>
#include <cassert>
#include <functional>
#include <string>
#include <thread>
#include <vector>

#include "../utils.h"
#include "simple_filter.h"

namespace wolf {

// ----------------------------------------------

LockFreeListFilter::LockFreeListFilter(const Input* input, usize num_threads)
    : input(input), num_threads(num_threads)
{
    assert(this->input != nullptr);

    output.clear();
    output.reserve(input->size());

    for (const auto& in : *input) {
        output.push_back(&in);
    }
}

const LockFreeListFilter::Output& LockFreeListFilter::filter(const std::string& pattern) {
    const usize size = input->size();
    usize chunck_size = size / num_threads;

    log_info("Parallel filter with %d threads and chucks of size %i\n", num_threads, chunck_size);

    std::vector<std::thread> threads;
    threads.reserve(num_threads);

    for (usize i = 0; i < num_threads; i++) {
        usize start = i * chunck_size;
        usize end = (i == num_threads - 1) ? size : (i + 1) * chunck_size;

        threads.emplace_back(
            LockFreeListFilter::process_workload,
            std::ref(output),
            std::ref(pattern),
            start,
            end
        );
    }

    for (auto& thread : threads) {
        thread.join();
    }

    auto it = std::remove_if(output.begin(), output.end(), [](const std::string* str) {
        return str == nullptr;
    });
    output.erase(it, output.end());

    return output;
}

void LockFreeListFilter::process_workload(Output& output, const std::string& pattern, usize start, usize end) {
    for (usize i = start; i < end; i++) {
        if (output[i]->find(pattern) != 0) {
            output[i] = nullptr;
        }
    }
}

// ----------------------------------------------

}
