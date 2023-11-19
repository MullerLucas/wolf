#include <iostream>
#include <stdexcept>

#include "incremental_filter_runner.h"
#include "../filter/multi_trie_filter.h"
#include "../gui/filter_window.h"

namespace wolf {

// ----------------------------------------------

IncrementalFilterRunner::IncrementalFilterRunner(const Config& config)
    : Runner(config), filter_(config_.thread_count), window_("hell-app", 640, 720, 32.0f)
{
    window_.create();
}

IncrementalFilterRunner::~IncrementalFilterRunner()
{
    window_.destroy();
}

void IncrementalFilterRunner::run()
{
    while (!window_.should_close()) {
        window_.draw();
    }
}

// ----------------------------------------------

}
