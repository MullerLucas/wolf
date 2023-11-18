#include <memory>
#include <stdexcept>

#include "benchmark_runner.h"
#include "generator_runner.h"
#include "oneshot_filter_runner.h"
#include "runner.h"

namespace wolf {

// ----------------------------------------------

Runner::Runner(const Config& config)
    : should_run_(true), config_(config)
{
    if (config.input_file.has_value()) {
        reader_ = new FileReader(config.input_file.value());
    } else {
        reader_ = new ConsoleReader();
    }

    if (config.output_file.has_value()) {
        writer_ = new FileWriter(config.output_file.value());
    } else {
        writer_ = new ConsoleWriter();
    }
}

Runner::~Runner() {
    delete reader_;
    delete writer_;
}

std::unique_ptr<Runner> Runner::create(const Config& config) {
    switch (config.run_mode) {
        case RunMode::FilterWords:
            log_info("Using OneShotFilterRunner\n");
            return std::make_unique<OneShotFilterRunner>(config);
        case RunMode::GenerateTestData:
            log_info("Using GeneratorRunner\n");
            return std::make_unique<GeneratorRunner>(config);
        case RunMode::Benchmark:
            log_info("Using BenchRunner\n");
            return std::make_unique<BenchmarkRunner>(config);
    }

    std::runtime_error("Unknown operation type");
}

// ----------------------------------------------

}
