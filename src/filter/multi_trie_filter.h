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
    void filter(MultiTrieFilterSession& session, const std::string& prefix);
    void collect(MultiTrieFilterSession& session) const;
    void stop();

private:
    usize                 thread_count_;
    ThreadPool            pool_;
    std::vector<WordTrie> tries_;
};

// ----------------------------------------------

}
