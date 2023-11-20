#include "oneshot_filter_runner.h"
#include "../filter/simple_vector_filter.h"
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
    auto filter = std::make_unique<SimpleVectorFilter>(config_->thread_count);
    auto session = filter->create_session(input);
    t.stop();
    log_info("Create filter: %zu us\n", t.elapsed_us().count());

    // run filter
    t.restart();
    filter->push(session, config_->prefix);
    t.stop();
    log_info("Running Filter: %zu us\n", t.elapsed_us().count());

    t.restart();
    filter->collect(session);
    t.stop();
    log_info("Collecting results: %zu us\n", t.elapsed_us().count());

    writer_->write_lines(session.filtered);
}

// ----------------------------------------------

}
