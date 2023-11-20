#include <string>
#include <vector>

#include "benchmark_runner.h"
#include "../benchmark.h"

namespace wolf {

// ----------------------------------------------

BenchmarkRunner::BenchmarkRunner(const Config *config)
    : Runner(config)
{ }

void BenchmarkRunner::run()
{
    const auto input = read_word_list();

    Benchmark bench(
        *writer_,
        input,
        { "A", "BC", "CAB", "ABCD", "ABCDE", "ABCDEF" },
        { 1, 2, 4, 8, 12, 16, 32, 48, 64 },
        config_->bench_iters
    );
    bench.run();
}

// ----------------------------------------------

}
