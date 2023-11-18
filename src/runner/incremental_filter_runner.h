#pragma once

#include <string>
#include <vector>

#include "runner.h"
#include "../filter/multi_trie_filter.h"

namespace wolf {

class IncrementalFilterRunner : public Runner {
public:
    IncrementalFilterRunner(const Config& config);

    void run() override;

private:
    std::vector<std::string> input_;
    MultiTrieFilter          filter_;
    MultiTrieFilterSession   session_;
};

}
