#include "config.h"
#include "utils.h"
#include <cstring>
#include <cwchar>
#include <cstdarg>

namespace wolf {

static bool compare_options(const char* str, const char* option, ...) {
    va_list args;
    va_start(args, option);

    const char* opt = va_arg(args, const char*);
    while (opt != nullptr) {
        if (strcmp(str, option) == 0) {
            va_end(args);
            return true;
        }
        opt = va_arg(args, const char*);
    }

    va_end(args);
    return false;
}

Config config_from_args(char** first, char** last) {
    Config config = DEFAULT_CONFIG;

    for (; first != last; ++first) {
        // operation-type
        if (compare_options(*first, "-f", "--filter")) {
            config.operation_type = OperationType::FilterWords;
        }
        else if (compare_options(*first, "-g", "--generate")) {
            config.operation_type = OperationType::GenerateTestData;
        }
        else if (compare_options(*first, "-h", "--help")) {
            config.operation_type = OperationType::Help;
        }
        // input-file
        else if (compare_options(*first, "-i", "--input")) {
            config.input_file = *++first;
        }
        // output-file
        else if (compare_options(*first, "-o", "--output")) {
            config.output_file = *++first;
        }
        // pattern
        else if (compare_options(*first, "-p", "--pattern")) {
            config.pattern = *++first;
        }
        // num-threads
        else if (compare_options(*first, "-t", "--threads")) {
            config.num_threads = std::stoi(*++first);
        }
        // width
        else if (compare_options(*first, "-w", "--width")) {
            config.width = std::stoi(*++first);
        }
        // is-incremental
        else if (compare_options(*first, "-i", "--incremental")) {
            config.is_incremental = true;
        }
        // should-shuffle
        else if (compare_options(*first, "-s", "--shuffle")) {
            config.should_shuffle = true;
        }
        // is-verbose
        else if (compare_options(*first, "-v", "--verbose")) {
            config.is_verbose = true;
        }
        else {
            config.pattern = *first;
        }
    }

    return config;
}

void log_config(const Config& config) {
    log_info("Config:\n");
    log_info("  operation_type: %d\n", config.operation_type);
    log_info("  input_file: %s\n",     config.input_file.value_or("nullopt").c_str());
    log_info("  output_file: %s\n",    config.output_file.value_or("nullopt").c_str());
    log_info("  pattern: %s\n",        config.pattern.c_str());
    log_info("  num_threads: %zu\n",   config.num_threads);
    log_info("  width: %zu\n",         config.width);
    log_info("  is_incremental: %d\n", config.is_incremental);
    log_info("  should_shuffle: %d\n", config.should_shuffle);
}

};
