#include "../utils.h"
#include "word_trie.h"
#include <functional>

namespace wolf {

// ----------------------------------------------

WordTrie::WordTrie() {
    root_ = new WordTrieNode();
    root_->key = '\0';
}

WordTrie::~WordTrie() {
    traverse_nodes_postorder(root_, [](WordTrieNode* node) {
        delete node;
    });
}

WordTrieState WordTrie::initial_state() const {
    return WordTrieState {
        .filtered_    = {},
        .curr_prefix_ = "",
        .curr_node    = root_
    };
}

void WordTrie::insert_all(const std::vector<std::string>& words) {
    for (const auto& w : words) {
        insert(w);
    }
}

bool WordTrie::insert(const std::string& word) {
    WordTrieNode* curr = root_;

    for (char ch : word) {
        if (curr->children.find(ch) == curr->children.end()) {
            curr->children[ch] = new WordTrieNode();
        }
        curr = curr->children[ch];
    }

    if (curr->word != nullptr) {
        return false;
    }

    curr->word = &word;
    return true;
}

void WordTrie::filter(WordTrieState state, const std::string& prefix) {
    const WordTrieNode* node = find(prefix);
    if (node) {
        collect_words_rec(state, node);
    }
}


const WordTrieNode* WordTrie::find(const std::string& prefix) const {
    WordTrieNode* curr = root_;

    for (char ch : prefix) {
        if (curr->children.find(ch) == curr->children.end()) {
            return nullptr;
        }
        curr = curr->children[ch];
    }

    return curr;
}

void WordTrie::collect_words_rec(WordTrieState& state, const WordTrieNode* node) const {
    if (node->word) {
        state.filtered_.push_back(node->word);
    }

    for (const auto& [key, child] : node->children) {
        collect_words_rec(state, child);
    }
}

void WordTrie::traverse_nodes_postorder(WordTrieNode* node, std::function<void(WordTrieNode*)> cb) {
    for (const auto& [key, child] : node->children) {
        traverse_nodes_postorder(child, cb);
    }
    cb(node);
}

// ----------------------------------------------

}

