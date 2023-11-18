#include "incremental_filter_runner.h"
#include "../filter/multi_trie_filter.h"
#include <iostream>

namespace wolf {

// ----------------------------------------------

IncrementalFilterRunner::IncrementalFilterRunner(const Config& config)
    : Runner(config), filter_(config_.thread_count)
{
    input_ = reader_->read_lines();
    assert(!input_.empty());
}

void IncrementalFilterRunner::setup() {
    session_ = filter_.create_session();
    filter_.insert_all(input_);
}

void IncrementalFilterRunner::run() {
    log_info("TODO\n");
    should_run_ = false;
}

// ----------------------------------------------

}
