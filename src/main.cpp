#include "benchmark.h"
#include "collections/word_trie.h"
#include "config.h"
#include "filter/multi_trie_filter.h"
#include "filter/simple_vector_filter.h"
#include "io/reader.h"
#include "io/writer.h"
#include "runner/benchmark_runner.h"
#include "runner/oneshot_filter_runner.h"
#include "runner/incremental_filter_runner.h"
#include "runner/runner.h"
#include "runner/generator_runner.h"
#include "testing.h"
#include "thread_pool.h"
#include "utils.h"

#include "benchmark.cpp"
#include "collections/word_trie.cpp"
#include "config.cpp"
#include "filter/multi_trie_filter.cpp"
#include "filter/simple_vector_filter.cpp"
#include "io/reader.cpp"
#include "io/writer.cpp"
#include "runner/benchmark_runner.cpp"
#include "runner/oneshot_filter_runner.cpp"
#include "runner/incremental_filter_runner.cpp"
#include "runner/runner.cpp"
#include "runner/generator_runner.cpp"
#include "testing.cpp"
#include "utils.cpp"

using namespace wolf;

// ----------------------------------------------

int main(int argc, char** argv) {
    // parse config
    Config config = config_from_args(argv + 1, argv + argc);
    if (config.is_verbose) {
        log_config(config);
    }

    if (config.run_mode == RunMode::Help) {
        log_help();
        return 0;
    }

    // run runners
    {
        Timer t;

        t.restart();
        auto runner = Runner::create(config);
        t.stop();
        log_info("Runner-Construction took %d us\n", t.elapsed_us());

        t.restart();
        runner->setup();
        t.stop();
        log_info("Runner-Setup took %d us\n", t.elapsed_us());

        while (runner->should_run_) {
            t.restart();
            runner->run();
            t.stop();
            log_info("Runner-Iteration took %d us\n", t.elapsed_us());
        }
    }

    return 0;
}

// ----------------------------------------------
