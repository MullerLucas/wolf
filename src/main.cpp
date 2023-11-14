#include <cassert>

#include "app.h"
#include "config.h"
#include "filter/filter.h"
#include "filter/lock_free_list_filter.h"
#include "io/reader.h"
#include "io/writer.h"
#include "testing.h"
#include "utils.h"

#include "app.cpp"
#include "config.cpp"
#include "filter/lock_free_list_filter.cpp"
#include "io/reader.cpp"
#include "io/writer.cpp"
#include "testing.cpp"
#include "utils.cpp"

using namespace wolf;

void run_filter_words(Config& config) {
    std::unique_ptr<Reader> reader = std::make_unique<FileReader>("data/input_w4_0.txt");
    auto input = reader->readln_all();
    assert(!input.empty());

    std::unique_ptr<Filter> filter = std::make_unique<LockFreeListFilter>(&input, config.num_threads);
    filter->filter(config.pattern);

    std::unique_ptr<Writer> writer = std::make_unique<ConsoleWriter>();
    writer->writeln_all(filter->create_output());
}

void run_generate_test_data(Config& config) {
    const auto test_data = generate_test_data(config.width, config.should_shuffle);

    std::unique_ptr<Writer> writer = std::make_unique<ConsoleWriter>();
    writer->writeln_all(test_data);
}

void run_help() {
    std::cout
    << "Usage: wolf [options] <pattern>" << "\n"
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
        case OperationType::Help:
            run_help();
            break;
    }

    return 0;
}
