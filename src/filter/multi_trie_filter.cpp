#include <algorithm>
#include <cassert>
#include <chrono>
#include <string>
#include <thread>
#include <vector>

#include "multi_trie_filter.h"

namespace wolf {

// ----------------------------------------------

MultiTrieFilter::MultiTrieFilter(usize thread_count)
    : thread_count_(thread_count), pool_(thread_count), tries_(thread_count)
{ }

MTFSession MultiTrieFilter::create_session() const
{
    MTFSession session;

    session.trie_sessions.reserve(thread_count_);
    for (usize i = 0; i < thread_count_; ++i)
        session.trie_sessions.emplace_back(tries_[i].create_session());

    return session;
}

// BUG(lm): data is only distributed equally across threads for one call of this
//          function calling this function multiple times with less data than
//          *thread_count_* will lead to sub-optimal distribution of words
void MultiTrieFilter::insert_all(const std::vector<std::string> &unfiltered)
{
    const usize target_chunk_size = std::max(MIN_CHUNK_SIZE_,
                                             unfiltered.size() / thread_count_);

    auto futures = ThreadPool::create_futures(thread_count_);

    for (usize i = 0; i < thread_count_; ++i) {
        usize start = i * target_chunk_size;

        // NOTE(lm): if there's not enough data to fill all threads, then we're done
        if (start >= unfiltered.size()) break;

        usize effective_chunk_size = std::min(target_chunk_size,
                                              unfiltered.size() - start);

        futures.emplace_back(
            pool_.enqueue([this, &unfiltered, start, effective_chunk_size, i] {
                const auto first = &unfiltered[start];
                tries_[i].insert_all(first, first + effective_chunk_size);
            })
        );
    }

    ThreadPool::await_futures(futures);
}

void MultiTrieFilter::push(MTFSession &session, const std::string &prefix)
{
    if (prefix.empty()) return;
    ++session.depth;

    auto futures = ThreadPool::create_futures(thread_count_);

    for (usize i = 0; i < thread_count_; ++i) {
        futures.emplace_back(
            pool_.enqueue([this, &session, &prefix, i]() {
                tries_[i].push(session.trie_sessions[i], prefix);
            })
        );
    }

    ThreadPool::await_futures(futures);
}

void MultiTrieFilter::pop(MTFSession &session, usize count)
{
    auto futures = ThreadPool::create_futures(thread_count_);

    session.depth = count > session.depth ? 0 : session.depth - count;

    for (usize i = 0; i < thread_count_; ++i) {
        futures.emplace_back(
            pool_.enqueue([this, &session, count, i]() {
                tries_[i].pop(session.trie_sessions[i], count);
            })
        );
    }

    ThreadPool::await_futures(futures);
}

void MultiTrieFilter::collect(MTFSession &session)
{
    usize total_size = 0;
    for (auto &ts : session.trie_sessions) {
        if (!ts.is_valid()) continue;
        total_size += ts.valid_node->word_count;
    }

    session.filtered_.resize(total_size, 0);
    if (total_size == 0) return;

    auto futures = ThreadPool::create_futures(thread_count_);
    usize offset = 0;

    for (usize i = 0; i < thread_count_; ++i) {
        if (!session.trie_sessions[i].is_valid()) continue;

        futures.emplace_back(
            pool_.enqueue([this, &session, offset, i] {
                tries_[i].collect(&session.trie_sessions[i], session.filtered_, offset);
            })
        );

        offset += session.trie_sessions[i].valid_node->word_count;
    }

    ThreadPool::await_futures(futures);
}


// ----------------------------------------------

}
