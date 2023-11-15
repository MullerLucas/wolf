#pragma once

#include <functional>
#include <optional>
#include <string>
#include <unordered_map>

#include "../utils.h"
#include "filter.h"

namespace wolf {

struct TrieFilterNode {
    char key;
    std::unordered_map<char, TrieFilterNode*> children;
    const std::string* word;
};

// ----------------------------------------------

class TrieFilter : public Filter {
public:
    TrieFilter(const Input* input);
    ~TrieFilter();

    void filter(const std::string& prefix) override;
    const Output& create_output() const  override { return output_; }

private:
    const Input* __attribute__((unused)) input_;
    Output output_;
    TrieFilterNode* root_;

    bool insert(const std::string& word);
    const TrieFilterNode* find(const std::string& prefix) const;
    void collect_words_rec(const TrieFilterNode* node, Output& output) const;

    static void traverse_nodes_postorder(TrieFilterNode* node, std::function<void(TrieFilterNode*)> cb);
};

// ----------------------------------------------

}
