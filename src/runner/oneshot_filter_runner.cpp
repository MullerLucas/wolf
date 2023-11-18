#include "oneshot_filter_runner.h"
#include "../filter/multi_trie_filter.h"

namespace wolf {

// ----------------------------------------------

OneShotFilterRunner::OneShotFilterRunner(const Config& config)
    : Runner(config), filter_(config_.thread_count)
{
    input_ = reader_->read_lines();
    assert(!input_.empty());
}

void OneShotFilterRunner::setup() {
    session_ = filter_.create_session();
    filter_.insert_all(input_);
}

void OneShotFilterRunner::run() {
    filter_.filter(session_, config_.prefix);
    filter_.collect(session_);

    writer_->write_lines(session_.filtered_);

    should_run_ = false;
}

// ----------------------------------------------

}
