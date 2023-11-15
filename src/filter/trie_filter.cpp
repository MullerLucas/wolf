#include "../utils.h"
#include "trie_filter.h"

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
    for (const auto& [key, child] : node->children) {
        if (child->word) {
            output.push_back(child->word);
        }
        collect_words_rec(child, output);
    }
}

// ----------------------------------------------

}

