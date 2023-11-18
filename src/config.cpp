#include "config.h"
#include "utils.h"
#include <cstring>
#include <cwchar>
#include <cstdarg>
#include <stdexcept>

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
    Config config {};

    for (; first != last; ++first) {
        // operation-type
        if (compare_options(*first, "-h", "--help")) {
            config.run_mode = RunMode::Help;
        }
        else if (compare_options(*first, "-m", "--mode")) {
            // filter-one-shot
            ++first;
            if (strcmp(*first, "fos") == 0) {
                config.run_mode = RunMode::FilterOneShot;
                config.prefix = *++first;
            }
            // filter-incremental
            else if (strcmp(*first, "fin") == 0) {
                config.run_mode = RunMode::FilterIncremental;
            }
            // generate test data
            else if (strcmp(*first, "gen") == 0) {
                config.run_mode = RunMode::GenerateTestData;
                config.gen_width = std::stoi(*++first);

                if (strcmp(*++first, "true") == 0) {
                    config.gen_shuffle = true;
                } else {
                    config.gen_shuffle = false;
                }
            }
            // benchmark
            else if (strcmp(*first, "bench") == 0) {
                config.run_mode = RunMode::Benchmark;

                // iterations
                config.bench_iters = std::stoi(*++first);
            }
        }
        // input-file
        else if (compare_options(*first, "-i", "--input")) {
            config.input_file = *++first;
        }
        // output-file
        else if (compare_options(*first, "-o", "--output")) {
            config.output_file = *++first;
        }
        // num-threads
        else if (compare_options(*first, "-t", "--threads")) {
            config.thread_count = std::stoi(*++first);
        }
        // is-verbose
        else if (compare_options(*first, "-v", "--verbose")) {
            config.is_verbose = true;
        }
        else {
            std::runtime_error("Unknown option");
        }
    }

    return config;
}

void log_config(const Config& config) {
    log_info("Config:\n");
    log_info("  run_mode:     %d\n",  config.run_mode);
    log_info("  input_file:   %s\n",  config.input_file.value_or("nullopt").c_str());
    log_info("  output_file:  %s\n",  config.output_file.value_or("nullopt").c_str());
    log_info("  prefix:       %s\n",  config.prefix.c_str());
    log_info("  thread_count: %zu\n", config.thread_count);
    log_info("  gen_width:    %zu\n", config.gen_width);
    log_info("  gen_shuffle:  %d\n",  config.gen_shuffle);
    log_info("  bench_iters:  %zu\n", config.bench_iters);
    log_info("  is_verbose:   %d\n",  config.is_verbose);
    log_info("\n");
}

void log_help() {
    std::cout
    << "Usage: wolf [options]\n"
    << "Options:\n"
    << "  -m, --mode         Run Mode\n"
    << "                     fos <prefix>         : filter: one-shot\n"
    << "                     fin                  : filter: incremental\n"
    << "                     gen <width> <suffle> : generate test data\n"
    << "                     bench <iter>         : benchmark\n"
    << "  -h, --help         Show help\n"
    << "  -i, --input        Input file\n"
    << "  -o, --output       Output file\n"
    << "  -p, --prefix       Prefix\n"
    << "  -t, --threads      Number of threads\n"
    << "  -i, --incremental  Incremental filter\n"
    << "  -s, --shuffle      Shuffle test data\n"
    << "  -v, --verbose      Verbose\n";
}

};

