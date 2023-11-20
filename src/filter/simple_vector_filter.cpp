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
    : thread_count_(thread_count), pool_(thread_count)
{ }

SVFSession SimpleVectorFilter::create_session(const std::vector<std::string> &unfiltered) const
{
    SVFSession session;
    session.filtered.clear();
    session.filtered.reserve(unfiltered.size());

    for (const auto& in : unfiltered)
        session.filtered.push_back(&in);

    return session;
}

void SimpleVectorFilter::push(SVFSession &session, const std::string &prefix)
{
    const usize size = session.filtered.size();
    const usize num_workloads = thread_count_ * WORKLOAD_MULTIPLIER;
    const usize chunk_size = size / num_workloads;

    const usize offset = session.prefix.size();
    session.prefix += prefix;

    auto futures = ThreadPool::create_futures(thread_count_);

    for (usize i = 0; i < num_workloads; ++i) {
        const usize start = i * chunk_size;
        const usize end = (i == num_workloads - 1) ? size : (i + 1) * chunk_size;

        futures.emplace_back(
            pool_.enqueue([=, &session] {
                push_slice(session.filtered, start, end, session.prefix, offset);
            })
        );
    }

    ThreadPool::await_futures(futures);
}

void SimpleVectorFilter::collect(SVFSession &session) const
{
    auto it = std::remove_if(session.filtered.begin(), session.filtered.end(),
        [](const std::string *str) {
            return str == nullptr;
        });
    session.filtered.erase(it, session.filtered.end());
}

void SimpleVectorFilter::push_slice(std::vector<const std::string*> &filtered,
                                    usize start, usize end,
                                    const std::string &prefix, usize offset)
{
    for (usize i = start; i < end; ++i) {
        if (filtered[i] != nullptr && !string_starts_with(*filtered[i], prefix, offset))
            filtered[i] = nullptr;
    }
}

// ----------------------------------------------

}
