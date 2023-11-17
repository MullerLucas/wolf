#include <algorithm>
#include <chrono>
#include <thread>
#include <vector>
#include <string>

#include "multi_trie_filter.h"

namespace wolf {

// ----------------------------------------------

MultiTrieFilter::MultiTrieFilter(usize thread_count)
    : thread_count_(thread_count), pool_(thread_count)
{
    tries_.reserve(thread_count);
    for (usize i = 0; i < thread_count; i++) {
        tries_.emplace_back();
    }
}

MultiTrieFilter::~MultiTrieFilter() { }

void MultiTrieFilter::insert_all(const std::vector<std::string>& unfiltered) {
    const usize target_chunk_size = unfiltered.size() / thread_count_;

    std::vector<std::future<void>> futures;

    for (usize i = 0; i < thread_count_; i++) {
        usize start = i * target_chunk_size;
        usize effective_chunk_size = std::min(target_chunk_size, unfiltered.size() - start);

        futures.emplace_back(
            pool_.enqueue([this, &unfiltered, start, effective_chunk_size, i] {
                const auto first = &unfiltered[start];
                tries_[i].insert_all(first, first + effective_chunk_size);
            })
        );
    }

    for (auto& future : futures) {
        future.wait();
    }
}

void MultiTrieFilter::filter(MultiTrieFilterSession& session, const std::string& prefix) {
    std::vector<std::future<void>> futures;

    for (usize i = 0; i < thread_count_; i++) {
        futures.emplace_back(
            pool_.enqueue([this, &session, &prefix, i]() {
                tries_[i].filter(&session.trie_sessions[i], prefix);
            })
        );
    }

    for (auto& future : futures) {
        future.wait();
    }

    session.filtered_.clear();
    for (auto& ts : session.trie_sessions) {
        session.filtered_.insert(session.filtered_.end(), ts.filtered_.begin(), ts.filtered_.end());
    }
}

MultiTrieFilterSession MultiTrieFilter::create_session() const {
    MultiTrieFilterSession session;

    session.trie_sessions.reserve(thread_count_);
    for (usize i = 0; i < thread_count_; i++) {
        session.trie_sessions.emplace_back(tries_[i].create_session());
    }

    return session;
}

// ----------------------------------------------

}
