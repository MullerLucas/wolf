#pragma once

#include "utils.h"
#include <optional>
#include <thread>

namespace wolf {

// ----------------------------------------------

enum class OperationType {
    FilterWords,
    GenerateTestData,
    Help,
};

// ----------------------------------------------

struct Config {
    OperationType operation_type;

    std::optional<std::string> input_file;
    std::optional<std::string> output_file;

    std::string pattern;

    usize num_threads;
    usize width;
    bool  is_incremental;
    bool  should_shuffle;
    bool  is_verbose;
};

const Config DEFAULT_CONFIG = {
    .operation_type = OperationType::FilterWords,
    .input_file     = std::nullopt,
    .output_file    = std::nullopt,
    .pattern        = "",
    .num_threads    = std::thread::hardware_concurrency(),
    .is_incremental = false,
    .should_shuffle = false,
    .is_verbose     = false,
};


Config config_from_args(char** first, char** last);
void   log_config(const Config& config);

// ----------------------------------------------

}
