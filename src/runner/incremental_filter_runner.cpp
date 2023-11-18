#include "incremental_filter_runner.h"
#include "../filter/multi_trie_filter.h"
#include <iostream>
#include <stdexcept>

namespace wolf {

// ----------------------------------------------

IncrementalFilterRunner::IncrementalFilterRunner(const Config& config)
    : Runner(config), filter_(config_.thread_count)
{ }

void IncrementalFilterRunner::run() {
    std::runtime_error("Unimplemented");
}

// ----------------------------------------------

}
