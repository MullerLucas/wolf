#include <cassert>
#include <memory>

#include "app.h"
#include "config.h"
#include "filter/filter.h"
#include "filter/lock_free_list_filter.h"
#include "io/reader.h"
#include "io/writer.h"
#include "testing.h"
#include "utils.h"
#include "filter/trie_filter.h"
#include "bench.h"

#include "app.cpp"
#include "config.cpp"
#include "filter/lock_free_list_filter.cpp"
#include "io/reader.cpp"
#include "io/writer.cpp"
#include "testing.cpp"
#include "utils.cpp"
#include "filter/trie_filter.cpp"
#include "bench.cpp"

using namespace wolf;

void run_filter_words(Config& config) {
    std::unique_ptr<Reader> reader = std::make_unique<FileReader>("data/input_w4_0.txt");
    auto input = reader->read_lines();
    assert(!input.empty());

    // std::unique_ptr<Filter> filter = std::make_unique<LockFreeListFilter>(&input, config.num_threads);
    // filter->filter(config.pattern);

    std::unique_ptr<TrieFilter> filter = std::make_unique<TrieFilter>(&input);
    filter->filter(config.prefix);

    std::unique_ptr<Writer> writer = std::make_unique<ConsoleWriter>();
    writer->write_lines(filter->create_output());
}

void run_benchmark() {
    std::unique_ptr<Reader> reader = std::make_unique<FileReader>("data/input_w4_1.txt");
    std::unique_ptr<Writer> writer = std::make_unique<ConsoleWriter>();

    auto input = reader->read_lines();
    assert(!input.empty());

    Bench bench(
        *writer,
        input,
        { "A", "BC", "CAB", "ABCD" },
        { 1, 2, 4, 8, 16, 32, 64 },
        50
    );
    bench.run();
}

void run_generate_test_data(Config& config) {
    const auto test_data = generate_test_data(config.width, config.should_shuffle);

    std::unique_ptr<Writer> writer = std::make_unique<ConsoleWriter>();
    writer->write_lines(test_data);
}

void run_help() {
    std::cout
    << "Usage: wolf [options] <prefix>" << "\n"
    << "Options:" << "\n"
    << "  -h, --help\t\t\tPrint this help message and exit" << "\n"
    << "  -i, --input <file>\t\tRead input from <file>" << "\n"
    << "  -o, --output <file>\t\tWrite output to <file>" << "\n"
    << "  -t, --threads <num>\t\tUse <num> threads" << "\n"
    << "  -v, --verbose\t\t\tPrint verbose output" <<
    std::endl;
}

int main(int argc, char** argv) {
    Config config = config_from_args(argv + 1, argv + argc);
    if (config.is_verbose) {
        log_config(config);
    }

    switch (config.operation_type) {
        case OperationType::FilterWords:
            run_filter_words(config);
            break;
        case OperationType::GenerateTestData:
            run_generate_test_data(config);
            break;
        case OperationType::Benchmark:
            run_benchmark();
            break;
        case OperationType::Help:
            run_help();
            break;
    }

    return 0;
}
