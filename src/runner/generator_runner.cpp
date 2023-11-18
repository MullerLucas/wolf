#pragma once

#include <string>
#include <vector>

#include "generator_runner.h"
#include "../bench.h"
#include "../testing.h"

namespace wolf {

// ----------------------------------------------

GeneratorRunner::GeneratorRunner(const Config& config)
    : Runner(config)
{ }

void GeneratorRunner::setup() { }

void GeneratorRunner::run() {
    const auto test_data = generate_test_data(config_.width, config_.should_shuffle);
    writer_->write_lines(test_data);
}

// ----------------------------------------------

}
