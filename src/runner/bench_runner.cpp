#pragma once

#include <string>
#include <vector>

#include "bench_runner.h"
#include "../bench.h"

namespace wolf {

// ----------------------------------------------

BenchRunner::BenchRunner(const Config& config)
    : Runner(config)
{
    input_ = reader_->read_lines();
    assert(!input_.empty());
}

void BenchRunner::setup() { }

void BenchRunner::run() {
    Bench bench(
        *writer_,
        input_,
        { "A", "BC", "CAB", "ABCD" },
        { 1, 2, 4, 8, 12, 16, 32, 48 },
        10
    );
    bench.run();
}

// ----------------------------------------------

}
