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
    clear();
}

WordTrieSession WordTrie::create_session() const {
    return WordTrieSession {
        .filtered    = {},
        .prefix = "",
        .node    = root_
    };
}

void WordTrie::insert_all(const std::string* first, const std::string* last) {
    for (; first != last; ++first) {
        (void)insert(*first);
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

void WordTrie::clear() {
    traverse_nodes_postorder(root_, [](WordTrieNode* node) {
        delete node;
    });
}

void WordTrie::filter(WordTrieSession* session, const std::string& prefix) const {
    session->prefix += prefix;
    session->node = find(session->node, prefix);
}

void WordTrie::collect(WordTrieSession* session) const {
    if (session->node) {
        collect_words_rec(session, session->node);
    }
}

const WordTrieNode* WordTrie::find(const WordTrieNode* node, const std::string& prefix) const {
    const WordTrieNode* curr = node;

    for (char ch : prefix) {
        if (curr->children.find(ch) == curr->children.end()) {
            return nullptr;
        }
        curr = curr->children.at(ch);
    }

    return curr;
}

void WordTrie::collect_words_rec(WordTrieSession* session, const WordTrieNode* node) const {
    if (node->word) {
        session->filtered.push_back(node->word);
    }

    for (const auto& [key, child] : node->children) {
        collect_words_rec(session, child);
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

