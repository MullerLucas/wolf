#pragma once

#include <string>
#include <vector>

#include "benchmark_runner.h"
#include "../benchmark.h"

namespace wolf {

// ----------------------------------------------

BenchmarkRunner::BenchmarkRunner(const Config& config)
    : Runner(config)
{
    input_ = reader_->read_lines();
    assert(!input_.empty());
}

void BenchmarkRunner::setup() { }

void BenchmarkRunner::run() {
    Benchmark bench(
        *writer_,
        input_,
        { "A", "BC", "CAB", "ABCD" },
        { 1, 2, 4, 8, 12, 16, 32, 48 },
        10
    );
    bench.run();

    should_run_ = false;
}

// ----------------------------------------------

}
