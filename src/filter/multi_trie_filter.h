#pragma once

#include <limits>
#include <vector>
#include <string>

#include "../collections/word_trie.h"
#include "../utils.h"
#include "../thread_pool.h"

namespace wolf {

// ----------------------------------------------

struct MTFSession {
    std::vector<const std::string*> filtered_;
    std::vector<WTSession>          trie_sessions;
    usize                           depth           = 0;
};

// ----------------------------------------------

class MultiTrieFilter {
public:
    MultiTrieFilter(usize thread_count);

    MTFSession create_session() const;

    void insert_all(const std::vector<std::string> &unfiltered);
    void push(MTFSession &session, const std::string &prefix);
    void pop(MTFSession &session, usize count);
    void collect(MTFSession &session);

private:
    static constexpr usize MIN_CHUNK_SIZE_ = 1;

    usize                   thread_count_;
    ThreadPool              pool_;
    std::vector<WordTrie>   tries_;

    void await_futures();
    std::vector<ThreadPool::Future> create_futures(usize thread_count);

};

// ----------------------------------------------

}
