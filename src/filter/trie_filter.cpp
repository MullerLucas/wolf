#include "../utils.h"
#include "trie_filter.h"
#include <functional>

namespace wolf {

// ----------------------------------------------

TrieFilter::TrieFilter(const Input* input)
    : input_(input)
{
    root_ = new TrieFilterNode();
    root_->key = '\0';

    for (const auto& word : *input) {
        insert(word);
    }
}

TrieFilter::~TrieFilter() {
    traverse_nodes_postorder(root_, [](TrieFilterNode* node) {
        delete node;
    });
}

void TrieFilter::filter(const std::string& prefix) {
    const TrieFilterNode* node = find(prefix);
    if (node) {
        collect_words_rec(node, output_);
    }
}

bool TrieFilter::insert(const std::string& word) {
    TrieFilterNode* curr = root_;

    for (char ch : word) {
        if (curr->children.find(ch) == curr->children.end()) {
            curr->children[ch] = new TrieFilterNode();
        }
        curr = curr->children[ch];
    }

    curr->word = &word;

    return true;
}

const TrieFilterNode* TrieFilter::find(const std::string& prefix) const {
    TrieFilterNode* curr = root_;

    for (char ch : prefix) {
        if (curr->children.find(ch) == curr->children.end()) {
            return nullptr;
        }
        curr = curr->children[ch];
    }

    return curr;
}

void TrieFilter::collect_words_rec(const TrieFilterNode* node, Output& output) const {
    if (node->word) {
        output.push_back(node->word);
    }

    for (const auto& [key, child] : node->children) {
        collect_words_rec(child, output);
    }
}

void TrieFilter::traverse_nodes_postorder(TrieFilterNode* node, std::function<void(TrieFilterNode*)> cb) {
    for (const auto& [key, child] : node->children) {
        traverse_nodes_postorder(child, cb);
    }
    cb(node);
}

// ----------------------------------------------

}

