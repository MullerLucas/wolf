#pragma once

#include <limits>
#include <vector>
#include <string>

#include "../collections/word_trie.h"
#include "../utils.h"
#include "../thread_pool.h"

namespace wolf {

// ----------------------------------------------

struct MultiTrieFilterStaticMetrics {
    i64 construct_time        = std::numeric_limits<i64>::max();
    i64 construct_thread_time = std::numeric_limits<i64>::max();

    i64 nodes_created = 0;
};

struct MultiTrieFilterSessionMetrics {
    i64 filter_time       = std::numeric_limits<i64>::max();
    i64 filter_trie_time  = std::numeric_limits<i64>::max();
    i64 filter_merge_time = std::numeric_limits<i64>::max();
};

// ----------------------------------------------

struct MultiTrieFilterSession {
    std::vector<const std::string*> filtered_;
    std::vector<WordTrieSession>    trie_sessions;

    MultiTrieFilterSessionMetrics   metrics_;
};

// ----------------------------------------------

class MultiTrieFilter {
public:
    MultiTrieFilter(usize thread_count);
    ~MultiTrieFilter();

    void insert_all(const std::vector<std::string>& unfiltered);
    void filter(MultiTrieFilterSession& session, const std::string& prefix);
    void stop();

    MultiTrieFilterSession create_session() const;


private:
    usize                 thread_count_;
    ThreadPool            pool_;
    std::vector<WordTrie> tries_;

    MultiTrieFilterStaticMetrics metrics_;
};

// ----------------------------------------------

}
