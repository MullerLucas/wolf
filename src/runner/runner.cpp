#include <memory>
#include <stdexcept>

#include "../io/console_reader.h"
#include "../io/console_writer.h"
#include "../io/file_reader.h"
#include "../io/file_writer.h"
#include "benchmark_runner.h"
#include "generator_runner.h"
#include "incremental_filter_runner.h"
#include "oneshot_filter_runner.h"
#include "runner.h"

namespace wolf {

// ----------------------------------------------

Runner::Runner(const Config *config)
    : config_(config)
{
    if (config->input_file.has_value())
        reader_ = new FileReader(config->input_file.value());
    else
        reader_ = new ConsoleReader();

    if (config->output_file.has_value())
        writer_ = new FileWriter(config->output_file.value());
    else
        writer_ = new ConsoleWriter();
}

Runner::~Runner()
{
    delete reader_;
    delete writer_;
}

std::unique_ptr<Runner> Runner::create(const Config *config)
{
    switch (config->run_mode) {
        case RunMode::FilterOneShot:
            return std::make_unique<OneShotFilterRunner>(config);
        case RunMode::FilterIncremental:
            return std::make_unique<IncrementalFilterRunner>(config);
        case RunMode::GenerateTestData:
            return std::make_unique<GeneratorRunner>(config);
        case RunMode::Benchmark:
            return std::make_unique<BenchmarkRunner>(config);
        default:
            std::runtime_error("Unknown operation type");
            break;
    }

    return nullptr;
}

std::vector<std::string> Runner::read_word_list()
{
    const auto input = reader_->read_lines();
    if (input.empty())
        std::runtime_error("Input is empty");
    return input;
}

// ----------------------------------------------

}
