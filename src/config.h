#pragma once

#include "utils.h"
#include <optional>
#include <thread>

namespace wolf {

// ----------------------------------------------

enum class RunMode {
    FilterWords,
    GenerateTestData,
    Benchmark,
};

// ----------------------------------------------

struct Config {
    RunMode run_mode = RunMode::FilterWords;

    std::optional<std::string> input_file  = std::nullopt;
    std::optional<std::string> output_file = std::nullopt;

    std::string prefix = "";

    usize thread_count  = std::thread::hardware_concurrency();
    usize gen_width     = 4;
    bool  bench_shuffle = false;
    usize bench_iters   = 10;
    bool  is_verbose    = false;
};

Config config_from_args(char** first, char** last);
void   log_config(const Config& config);
void   log_help();

// ----------------------------------------------

}
