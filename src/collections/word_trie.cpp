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
        .prefix = "",
        .node    = root_
    };
}

// NOTE(lm): assumes words are not already in trie (no duplicates)
void WordTrie::insert_all(const std::string* first, const std::string* last) {
    for (; first != last; ++first) {
        (void)insert(*first);
    }
}

// NOTE(lm): assumes word is not already in trie (no duplicates)
void WordTrie::insert(const std::string& word) {
    WordTrieNode* curr = root_;

    for (char ch : word) {
        if (curr->children.find(ch) == curr->children.end()) {
            curr->children[ch] = new WordTrieNode();
        }
        curr->word_count += 1;
        curr = curr->children[ch];
    }

    curr->word_count += 1;
    curr->word = &word;
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

void WordTrie::collect(WordTrieSession* session, std::vector<const std::string*>& collector, usize offset) const {
    if (session->node) {
        collect_words_rec(session, session->node, collector, offset);
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

void WordTrie::collect_words_rec(
    WordTrieSession* session,
    const WordTrieNode* node,
    std::vector<const std::string*>& collector,
    usize& offset
) const {
    if (node->word) {
        collector[offset] = node->word;
        offset += 1;
    }

    for (const auto& [key, child] : node->children) {
        collect_words_rec(session, child, collector, offset);
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

