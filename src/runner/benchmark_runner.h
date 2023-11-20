#pragma once

#include "runner.h"

namespace wolf {

// ----------------------------------------------

class BenchmarkRunner : public Runner {
public:
    BenchmarkRunner(const Config *config);

    void run() override;
};

// ----------------------------------------------

}
