#include "oneshot_filter_runner.h"
#include "../filter/multi_trie_filter.h"
#include <stdexcept>

namespace wolf {

// ----------------------------------------------

OneShotFilterRunner::OneShotFilterRunner(const Config *config)
    : Runner(config)
{ }

void OneShotFilterRunner::run()
{
    Timer t;

    const auto input = read_word_list();

    // create filter
    t.restart();
    auto filter  = std::make_unique<MultiTrieFilter>(config_->thread_count);
    auto session = filter->create_session();
    t.stop();
    log_info("Create filter: %zu us\n", t.elapsed_us().count());

    // insert data
    t.restart();
    filter->insert_all(input);
    t.stop();
    log_info("Insert data: %zu us\n", t.elapsed_us().count());

    // run filter
    t.restart();
    filter->push(session, config_->prefix);
    filter->collect(session);
    t.stop();
    log_info("Running Filter: %zu us\n", t.elapsed_us().count());

    t.restart();
    filter->collect(session);
    t.stop();
    log_info("Collecting results: %zu us\n", t.elapsed_us().count());

    writer_->write_lines(session.filtered_);
}

// ----------------------------------------------

}
