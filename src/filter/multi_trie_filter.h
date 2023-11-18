#pragma once

#include <limits>
#include <vector>
#include <string>

#include "../collections/word_trie.h"
#include "../utils.h"
#include "../thread_pool.h"

namespace wolf {

// ----------------------------------------------

struct MultiTrieFilterSession {
    std::vector<const std::string*> filtered_;
    std::vector<WordTrieSession>    trie_sessions;
};

// ----------------------------------------------

class MultiTrieFilter {
public:
    MultiTrieFilter(usize thread_count);

    MultiTrieFilterSession create_session() const;

    void insert_all(const std::vector<std::string>& unfiltered);
    void push_filter(MultiTrieFilterSession& session, const std::string& prefix);
    void pop_filter(MultiTrieFilterSession& session, usize count);
    void collect(MultiTrieFilterSession& session);

private:
    const usize MIN_CHUNK_SIZE_ = 1;

    usize                   thread_count_;
    ThreadPool              pool_;
    std::vector<WordTrie>   tries_;

    void await_futures();
    std::vector<ThreadPool::Future> create_futures(usize thread_count);

};

// ----------------------------------------------

}
