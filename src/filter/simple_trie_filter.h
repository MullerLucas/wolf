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

class SimpleTrieFilter : public Filter {
public:
    SimpleTrieFilter(const InVec* input);
    ~SimpleTrieFilter();

    void filter(const std::string& prefix);
    const OutVec& create_output() const { return output_; }

private:
    const InVec* __attribute__((unused)) input_;
    OutVec output_;
    TrieFilterNode* root_;

    bool insert(const std::string& word);
    const TrieFilterNode* find(const std::string& prefix) const;
    void collect_words_rec(const TrieFilterNode* node, OutVec& output) const;

    static void traverse_nodes_postorder(TrieFilterNode* node, std::function<void(TrieFilterNode*)> cb);
};

// ----------------------------------------------

}
