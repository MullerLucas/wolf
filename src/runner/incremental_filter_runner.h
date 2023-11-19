#pragma once

#include <string>
#include <vector>

#include "runner.h"
#include "../filter/multi_trie_filter.h"
#include "../gui/filter_window.h"

namespace wolf {

// ----------------------------------------------

class IncrementalFilterRunner : public Runner {
public:
    IncrementalFilterRunner(const Config& config);
    ~IncrementalFilterRunner() override;

    void run() override;

private:
    std::vector<std::string>    input_;
    MultiTrieFilter             filter_;
    MultiTrieFilterSession      session_;

    FilterWindow                window_;
};

// ----------------------------------------------

}
