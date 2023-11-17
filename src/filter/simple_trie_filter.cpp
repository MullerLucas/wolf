#include "../utils.h"
#include "simple_trie_filter.h"
#include <functional>

namespace wolf {

// ----------------------------------------------

SimpleTrieFilter::SimpleTrieFilter(const InVec* input)
    : input_(input)
{
    root_ = new TrieFilterNode();
    root_->key = '\0';

    for (const auto& word : *input) {
        insert(word);
    }
}

SimpleTrieFilter::~SimpleTrieFilter() {
    traverse_nodes_postorder(root_, [](TrieFilterNode* node) {
        delete node;
    });
}

void SimpleTrieFilter::filter(const std::string& prefix) {
    const TrieFilterNode* node = find(prefix);
    if (node) {
        collect_words_rec(node, output_);
    }
}

bool SimpleTrieFilter::insert(const std::string& word) {
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

const TrieFilterNode* SimpleTrieFilter::find(const std::string& prefix) const {
    TrieFilterNode* curr = root_;

    for (char ch : prefix) {
        if (curr->children.find(ch) == curr->children.end()) {
            return nullptr;
        }
        curr = curr->children[ch];
    }

    return curr;
}

void SimpleTrieFilter::collect_words_rec(const TrieFilterNode* node, OutVec& output) const {
    if (node->word) {
        output.push_back(node->word);
    }

    for (const auto& [key, child] : node->children) {
        collect_words_rec(child, output);
    }
}

void SimpleTrieFilter::traverse_nodes_postorder(TrieFilterNode* node, std::function<void(TrieFilterNode*)> cb) {
    for (const auto& [key, child] : node->children) {
        traverse_nodes_postorder(child, cb);
    }
    cb(node);
}

// ----------------------------------------------

}

