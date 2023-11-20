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

SimpleVectorFilter::SimpleVectorFilter(usize thread_count)
    : thread_count_(thread_count), pool_(thread_count), input_(nullptr), prefix_("")
{ }

void SimpleVectorFilter::insert_all(const std::vector<std::string> *input)
{
    this->input_ = input;

    output_.clear();
    output_.reserve(input_->size());

    for (const auto& in : *input_)
        output_.push_back(&in);
}

void SimpleVectorFilter::push(const std::string &prefix)
{
    const usize size = output_.size();
    const usize num_workloads = thread_count_ * WORKLOAD_MULTIPLIER;
    const usize chunk_size = size / num_workloads;

    const usize offset = prefix_.size();
    prefix_ += prefix;

    auto futures = ThreadPool::create_futures(thread_count_);

    for (usize i = 0; i < num_workloads; ++i) {
        const usize start = i * chunk_size;
        const usize end   = (i == num_workloads - 1) ? size : (i + 1) * chunk_size;

        futures.emplace_back(
            pool_.enqueue([this, start, end, offset] {
                process_workload(output_, start, end, prefix_, offset);
            })
        );
    }

    ThreadPool::await_futures(futures);

    auto it = std::remove_if(output_.begin(), output_.end(), [](const std::string *str) {
        return str == nullptr;
    });
    output_.erase(it, output_.end());
}

const std::vector<const std::string*> &SimpleVectorFilter::collect() const
{
    return output_;
}

void SimpleVectorFilter::process_workload(std::vector<const std::string*> &output,
                                          usize start, usize end,
                                          const std::string &prefix, usize offset)
{
    for (usize i = start; i < end; ++i) {
        if (!string_starts_with(*output[i], prefix, offset))
            output[i] = nullptr;
    }
}

// ----------------------------------------------

}
