#pragma once

#include "generator_runner.h"
#include "../testing.h"

namespace wolf {

// ----------------------------------------------

GeneratorRunner::GeneratorRunner(const Config *config)
    : Runner(config)
{ }

void GeneratorRunner::run() {
    const auto test_data = generate_test_data(config_->gen_width, config_->gen_shuffle);
    writer_->write_lines(test_data);
}

// ----------------------------------------------

}
