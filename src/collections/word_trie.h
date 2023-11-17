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
    char key;
    std::unordered_map<char, WordTrieNode*> children;
    const std::string* word;
};

// ----------------------------------------------

struct WordTrieMetrics {
    i64 construct_time;
    i64 filter_time;
    i64 collect_time;

    i64 total_time() const {
        return construct_time + filter_time + collect_time;
    }
};

// ----------------------------------------------

struct WordTrieState {
    std::vector<const std::string*> filtered_;

    std::string   curr_prefix_;
    WordTrieNode* curr_node;
};

// ----------------------------------------------

class WordTrie {
public:
    WordTrie();
    ~WordTrie();

    WordTrieState initial_state() const;

    void insert_all(const std::vector<std::string>& words);
    bool insert(const std::string& word);
    void clear();

    void filter(WordTrieState state, const std::string& prefix);

private:
    WordTrieNode* root_;

    const WordTrieNode* find(const std::string& prefix) const;
    void collect_words_rec(WordTrieState& state, const WordTrieNode* node) const;

    static void traverse_nodes_postorder(WordTrieNode* node, std::function<void(WordTrieNode*)> cb);
};

// ----------------------------------------------

}
