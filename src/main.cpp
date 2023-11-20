#include "config.h"
#include "runner/runner.h"

#include "benchmark.cpp"
#include "collections/word_trie.cpp"
#include "config.cpp"
#include "filter/multi_trie_filter.cpp"
#include "filter/simple_vector_filter.cpp"
#include "gui/filter_window.cpp"
#include "io/console_reader.cpp"
#include "io/console_writer.cpp"
#include "io/file_reader.cpp"
#include "io/file_writer.cpp"
#include "io/writer.cpp"
#include "runner/benchmark_runner.cpp"
#include "runner/generator_runner.cpp"
#include "runner/incremental_filter_runner.cpp"
#include "runner/oneshot_filter_runner.cpp"
#include "runner/runner.cpp"
#include "testing.cpp"
#include "utils.cpp"

using namespace wolf;

// ----------------------------------------------

int main(int argc, char **argv)
{
    // parse config
    Config config = config_from_args(argv + 1, argv + argc);
    if (config.is_verbose)
        log_config(config);

    // show help, if needed
    if (config.run_mode == RunMode::Help) {
        log_help();
        return 0;
    }

    // run runners
    {
        auto runner = Runner::create(&config);
        runner->run();
    }

    return 0;
}

// ----------------------------------------------
