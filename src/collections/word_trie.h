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
    char               key;
    const std::string* word;
    usize              word_count = 0;
};

// ----------------------------------------------

struct WordTrieSession {
    std::string         prefix;
    const WordTrieNode* node;
};

// ----------------------------------------------

class WordTrie {
public:
    WordTrie();
    ~WordTrie();

    WordTrieSession create_session() const;

    void insert_all(const std::string* first, const std::string* last);
    void insert(const std::string& word);
    void clear();

    void filter (WordTrieSession* session, const std::string& prefix) const;
    void collect(WordTrieSession* session, std::vector<const std::string*>& collector, usize offset) const;

private:
    WordTrieNode* root_;

    const WordTrieNode* find(const WordTrieNode* node, const std::string& prefix) const;
    void collect_words_rec(
        WordTrieSession* session,
        const WordTrieNode* node,
        std::vector<const std::string*>& collector,
        usize& offset
    ) const;

    static void traverse_nodes_postorder(WordTrieNode* node, std::function<void(WordTrieNode*)> cb);
};

// ----------------------------------------------

}
