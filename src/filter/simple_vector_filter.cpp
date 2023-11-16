#include <algorithm>
#include <cassert>
#include <functional>
#include <string>
#include <thread>
#include <vector>

#include "../utils.h"
#include "simple_vector_filter.h"
#include "../thread_pool.h"

namespace wolf {

// ----------------------------------------------

SimpleVectorFilter::SimpleVectorFilter(usize num_threads)
    : input_(nullptr), prefix_(""), num_threads_(num_threads)
{
    pool_.start(num_threads_);
}

void SimpleVectorFilter::init_data(const InVec* input) {
    this->input_ = input;

    output_.clear();
    output_.reserve(input_->size());

    for (const auto& in : *input_) {
        output_.push_back(&in);
    }

    prefix_.clear();
}

const usize WORKLOAD_MULTIPLIER = 3;

void SimpleVectorFilter::filter(const std::string& prefix) {
    const usize size          = output_.size();
    const usize num_workloads = num_threads_ * WORKLOAD_MULTIPLIER;
    const usize chunck_size   = size / num_workloads;

    const usize offset = prefix_.size();
    prefix_ += prefix;

    std::vector<std::thread> threads;
    threads.reserve(num_threads_);


    for (usize i = 0; i < num_workloads; i++) {
        usize start = i * chunck_size;
        usize end   = (i == num_workloads - 1) ? size : (i + 1) * chunck_size;

        // threads.emplace_back(
        //     SimpleVectorFilter::process_workload,
        //     std::ref(output_),
        //     start,
        //     end,
        //     std::ref(prefix_),
        //     offset
        // );

        pool_.queue_job([this, start, end, offset] {
            process_workload(
                output_,
                start,
                end,
                prefix_,
                offset
            );
        });
    }

    // for (auto& thread : threads) {
    //     thread.join();
    // }

    pool_.stop();

    auto it = std::remove_if(output_.begin(), output_.end(), [](const std::string* str) {
        return str == nullptr;
    });
    output_.erase(it, output_.end());
}

const SimpleVectorFilter::OutVec& SimpleVectorFilter::create_output() const {
    return output_;
}

void SimpleVectorFilter::process_workload(
    OutVec& output,
    usize   start,
    usize   end,
    const   std::string& prefix,
    usize   offset
) {
    for (usize i = start; i < end; i++) {
        if (!string_starts_with(*output[i], prefix, offset)) {
            output[i] = nullptr;
        }
    }
}

// ----------------------------------------------

}
