#include "oneshot_filter_runner.h"
#include "../filter/multi_trie_filter.h"

namespace wolf {

// ----------------------------------------------

OneShotFilterRunner::OneShotFilterRunner(const Config& config)
    : Runner(config), filter_(config_.num_threads)
{
    input_ = reader_->read_lines();
    assert(!input_.empty());
}

void OneShotFilterRunner::setup() {
    filter_.insert_all(input_);
}

void OneShotFilterRunner::run() {
    auto session = filter_.create_session();
    filter_.filter(session, config_.prefix);
    filter_.collect(session);

    writer_->write_lines(session.filtered_);

    should_run_ = false;
}

// ----------------------------------------------

}
