#pragma once

#include <functional>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include "../utils.h"

namespace wolf {

// ----------------------------------------------

struct WordTrieNode {
    std::unordered_map<char, WordTrieNode*> children;
    WordTrieNode                            *parent             = nullptr;
    const                                   std::string *word   = nullptr;
    usize                                   word_count          = 0;
};

// ----------------------------------------------

struct WTSession {
    std::string         valid_prefix    = "";
    const WordTrieNode  *valid_node     = nullptr;
    usize               depth           = 0;

    bool is_valid() const {
        return depth <= valid_prefix.size();
    }
};

// ----------------------------------------------

class WordTrie {
public:
    WordTrie();
    ~WordTrie();

    WordTrie(const WordTrie &other);
    WordTrie &operator=(const WordTrie &other);
    WordTrie(WordTrie &&other);
    WordTrie &operator=(WordTrie &&other);

    WTSession create_session() const;

    void insert_all(const std::string *first, const std::string *last);
    void insert(const std::string &word);
    void clear();

    void push(WTSession &session, const std::string &prefix) const;
    void pop(WTSession &session, usize count) const;
    void collect(WTSession *session, std::vector<const std::string*> &collector,
                 usize offset) const;

private:
    WordTrieNode *root_;

    const WordTrieNode *find(const WordTrieNode *node,
                             const std::string &prefix) const;
    void collect_rec(WTSession *session, const WordTrieNode *node,
                     std::vector<const std::string*> &collector,
                     usize &offset) const;

    static void traverse_postorder(WordTrieNode *node,
                                   std::function<void(WordTrieNode*)> cb);
};

// ----------------------------------------------

}
